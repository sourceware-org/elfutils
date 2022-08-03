#!/usr/bin/env bash
#
# Copyright (C) 2019-2021 Red Hat, Inc.
# This file is part of elfutils.
#
# This file is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# elfutils is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

. $srcdir/debuginfod-subr.sh

type rpmsign 2>/dev/null || { echo "need rpmsign"; exit 77; }
cat << EoF > include.c 
#include <rpm/rpmlib.h>
#include <rpm/rpmfi.h>
#include <rpm/header.h>
#include <imaevm.h>
#include <openssl/evp.h>
EoF
tempfiles include.c 
gcc -H -fsyntax-only include.c 2> /dev/null || { echo "one or more devel packages are missing (rpm-devel, ima-evm-utils-devel, openssl-devel)"; exit 77; }

DB=${PWD}/.debuginfod_tmp.sqlite
tempfiles $DB
export DEBUGINFOD_CACHE_PATH=${PWD}/.client_cache
export DEBUGINFOD_IMA_POLICY="require"

# This variable is essential and ensures no time-race for claiming ports occurs
# set base to a unique multiple of 100 not used in any other 'run-debuginfod-*' test
base=13000
get_ports
mkdir R
env LD_LIBRARY_PATH=$ldpath DEBUGINFOD_URLS= ${abs_builddir}/../debuginfod/debuginfod $VERBOSE -R \
    -d $DB -p $PORT1 -t0 -g0 R > vlog$PORT1 2>&1 &
PID1=$!
tempfiles vlog$PORT1
errfiles vlog$PORT1

########################################################################
cp -pv ${abs_srcdir}/debuginfod-ima/rhel9/hello2-1.0-1.x86_64.rpm signed.rpm
tempfiles signed.rpm
RPM_BUILDID=460912dbc989106ec7325d243384df20c5ccec0c # /usr/local/bin/hello

MIN_IMAEVM_MAJ_VERSION=3
MIN_RPM_MAJ_VERSION=4
# If the correct programs (and versions) exist sign the rpm in the test
if  (command -v openssl &> /dev/null) && \
    (command -v rpmsign &> /dev/null) && \
    (command -v gpg &> /dev/null) && \
    [ $(ldd /bin/rpmsign | grep libimaevm | awk -F'[^0-9]+' '{ print $2 }') -ge $MIN_IMAEVM_MAJ_VERSION ] && \
    [ $(rpm --version | awk -F'[^0-9]+' '{ print $2 }') -ge $MIN_RPM_MAJ_VERSION ]
then
    # SIGN THE RPM
    # First remove any old signatures
    rpmsign --delsign signed.rpm &> /dev/null
    rpmsign --delfilesign signed.rpm &> /dev/null

    # Make a gpg keypair (with $PWD as the homedir)
    mkdir -m 700 openpgp-revocs.d private-keys-v1.d
    gpg --quick-gen-key --yes --homedir ${PWD} --batch --passphrase '' --no-default-keyring --keyring "${PWD}/pubring.kbx" example@redhat.com 2> /dev/null

    # Create a private DER signing key and a public X509 DER format verification key pair
    openssl genrsa | openssl pkcs8 -topk8 -nocrypt -outform PEM -out signing.pem
    openssl req -x509 -key signing.pem -out imacert.pem -days 365 -keyform PEM \
        -subj "/C=CA/ST=ON/L=TO/O=Redhat/CN=www.redhat.com"
    openssl x509 -in imacert.pem -inform PEM -out imacert.der -outform DER

    tempfiles openpgp-revocs.d/* private-keys-v1.d/* * openpgp-revocs.d private-keys-v1.d

    rpmsign --addsign --signfiles --fskpath=signing.pem -D "_gpg_name example@redhat.com" -D "_gpg_path ${PWD}" signed.rpm
    cp signed.rpm R/signed.rpm
    VERIFICATION_CERT=${PWD}/imacert.der

    # Cleanup
    rm -rf openpgp-revocs.d private-keys-v1.d
else
    # USE A PRESIGNED RPM
    cp signed.rpm R/signed.rpm
    VERIFICATION_CERT=${abs_srcdir}/debuginfod-ima/rhel9/imacert.der
fi

########################################################################
# Server must become ready with R fully scanned and indexed
wait_ready $PORT1 'ready' 1
wait_ready $PORT1 'thread_work_total{role="traverse"}' 1
wait_ready $PORT1 'thread_work_pending{role="scan"}' 0
wait_ready $PORT1 'thread_busy{role="scan"}' 0

export DEBUGINFOD_URLS=http://127.0.0.1:$PORT1

# Test 1: Without a certificate the verification should fail
RC=0
testrun ${abs_top_builddir}/debuginfod/debuginfod-find executable $RPM_BUILDID || RC=1
test $RC -ne 0

# Test 2: It should pass once the certificate is added to the path
export DEBUGINFOD_IMA_CERT_PATHS=$VERIFICATION_CERT
rm -rf $DEBUGINFOD_CACHE_PATH # clean it from previous tests
kill -USR1 $PID1
wait_ready $PORT1 'thread_work_total{role="traverse"}' 2
wait_ready $PORT1 'thread_work_pending{role="scan"}' 0
wait_ready $PORT1 'thread_busy{role="scan"}' 0
testrun ${abs_top_builddir}/debuginfod/debuginfod-find -vv executable $RPM_BUILDID

# Test 3: Corrupt the data and it should fail
dd if=/dev/zero of=R/signed.rpm bs=1 count=128 seek=1024 conv=notrunc
rm -rf $DEBUGINFOD_CACHE_PATH # clean it from previous tests
kill -USR1 $PID1
wait_ready $PORT1 'thread_work_total{role="traverse"}' 3
wait_ready $PORT1 'thread_work_pending{role="scan"}' 0
wait_ready $PORT1 'thread_busy{role="scan"}' 0
RC=0
testrun ${abs_top_builddir}/debuginfod/debuginfod-find executable $RPM_BUILDID || RC=1
test $RC -ne 0

# Test 4: A rpm without a signature will fail
cp signed.rpm R/signed.rpm
rpmsign --delfilesign R/signed.rpm
rm -rf $DEBUGINFOD_CACHE_PATH # clean it from previous tests
kill -USR1 $PID1
wait_ready $PORT1 'thread_work_total{role="traverse"}' 4
wait_ready $PORT1 'thread_work_pending{role="scan"}' 0
wait_ready $PORT1 'thread_busy{role="scan"}' 0
RC=0
testrun ${abs_top_builddir}/debuginfod/debuginfod-find executable $RPM_BUILDID || RC=1
test $RC -ne 0

# Test 5: Only tests 1,2 will result in extracted signature
[[ $(curl -s http://127.0.0.1:$PORT1/metrics | grep 'http_responses_total{extra="ima-sigs-extracted"}' | awk '{print $NF}') -eq 2 ]]

kill $PID1
wait $PID1
PID1=0
exit 0
