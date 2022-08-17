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

# for test case debugging, uncomment:
set -x
unset VALGRIND_CMD

type curl 2>/dev/null || { echo "need curl"; exit 77; }
type jq 2>/dev/null || { echo "need jq"; exit 77; }

pkg-config json-c libcurl || { echo "one or more libraries are missing (libjson-c, libcurl)"; exit 77; }

DB=${PWD}/.debuginfod_tmp.sqlite
export DEBUGINFOD_CACHE_PATH=${PWD}/.client_cache
tempfiles $DB ${DB}_2

# This variable is essential and ensures no time-race for claiming ports occurs
# set base to a unique multiple of 100 not used in any other 'run-debuginfod-*' test
base=13100
get_ports
mkdir R D
cp -rvp ${abs_srcdir}/debuginfod-rpms/rhel7 R
cp -rvp ${abs_srcdir}/debuginfod-debs/*deb D

env LD_LIBRARY_PATH=$ldpath DEBUGINFOD_URLS= ${abs_builddir}/../debuginfod/debuginfod $VERBOSE -R \
    -d $DB -p $PORT1 -t0 -g0 R > vlog$PORT1 2>&1 &
PID1=$!
tempfiles vlog$PORT1
errfiles vlog$PORT1

wait_ready $PORT1 'ready' 1
wait_ready $PORT1 'thread_work_total{role="traverse"}' 1
wait_ready $PORT1 'thread_work_pending{role="scan"}' 0
wait_ready $PORT1 'thread_busy{role="scan"}' 0

env LD_LIBRARY_PATH=$ldpath DEBUGINFOD_URLS="http://127.0.0.1:$PORT1 https://bad/url.web" ${abs_builddir}/../debuginfod/debuginfod $VERBOSE -U \
    -d ${DB}_2 -p $PORT2 -t0 -g0 D > vlog$PORT2 2>&1 &
PID2=$!
tempfiles vlog$PORT2
errfiles vlog$PORT2

wait_ready $PORT2 'ready' 1
wait_ready $PORT2 'thread_work_total{role="traverse"}' 1
wait_ready $PORT2 'thread_work_pending{role="scan"}' 0
wait_ready $PORT2 'thread_busy{role="scan"}' 0

# have clients contact the new server
export DEBUGINFOD_URLS=http://127.0.0.1:$PORT2

tempfiles json.txt
# Check that we find 11 files(which means that the local and upstream correctly reply to the query)
N_FOUND=`${abs_top_builddir}/debuginfod/debuginfod-find metadata "/?sr*" | jq '. | length'`
test $N_FOUND -eq 11

# Query via the webapi as well
MTIME=$(stat -c %Y D/hithere_1.0-1_amd64.deb)
EXPECTED='[ { "atype": "e", "buildid": "f17a29b5a25bd4960531d82aa6b07c8abe84fa66", "mtime": "'$MTIME'", "stype": "R", "source0": "'$PWD'/D/hithere_1.0-1_amd64.deb", "source1": "/usr/bin/hithere"} ]'
test `curl http://127.0.0.1:$PORT2/metadata?glob=/usr/bin/*hi* | jq ". == $EXPECTED" ` = 'true'

# An empty array is returned on server error or if the file DNE
test `${abs_top_builddir}/debuginfod/debuginfod-find metadata "/this/isnt/there" | jq ". == [ ]" ` = 'true'

kill $PID1
kill $PID2
wait $PID1
wait $PID2
PID1=0
PID2=0

test `${abs_top_builddir}/debuginfod/debuginfod-find metadata "/usr/bin/hithere" | jq ". == [ ]" ` = 'true'

exit 0
