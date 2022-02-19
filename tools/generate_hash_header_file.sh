generate_hash_header(){
	local _commitHash
	_commitHash=$(git log -n 1 --pretty=format:"%h")
	local _dirty
	_dirty=$(git describe --dirty|grep -o dirty$)
	if [ x"${_dirty}" = x ]
	then
		echo "#define CI_INFO \"$_commitHash\""
	else
		echo "#define CI_INFO \"$_commitHash-$_dirty\""
	fi
}

generate_hash_header
