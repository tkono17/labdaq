#----------------------------------------------------------------
# setup_caen.sh
#----------------------------------------------------------------
if [[ ${TK_ROOT}a == "a" ]]; then
    export TK_ROOT=/home/tkohno/work/tk
    if [[ ${TKDEV_ROOT}a == "a" ]]; then
	export TKDEV_ROOT=${TK_ROOT}/dev
    fi
fi
export PATH=${PWD}/exe:${PATH}
export LD_LIBRARY_PATH=${PWD}/lib:${LD_LIBRARY_PATH}
