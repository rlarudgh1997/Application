#! /usr/bin/env bash

set -e

script_file=$(readlink -f "$0")
script_path=$(readlink -m "$(dirname "${script_file}")")
sfc_path="$script_path/.."

if [ ! -d ${sfc_path} ]; then
	echo "ERROR: No sfc directory."
	exit 1
fi

clang_format_version="13"

if [[ -x $(which clang-format-${clang_format_version}) ]] &&
	[[ $(clang-format-${clang_format_version} --version 2>&1) =~ clang-format* ]]; then
	clang_format="clang-format-${clang_format_version}"
elif [[ -x $(which clang-format) ]] &&
	[[ $(clang-format --version 2>&1) =~ clang-format* ]]; then
	clang_format="clang-format"
else
	cat <<EOS
[ERROR] Please install clang-format.
EOS
	exit 1
fi

sfc_list=()
flag_fullformat=false
flag_dryrun=false
flag_apply=false

while test $# -gt 0; do
	case "$1" in
	-f | --full)
		flag_fullformat=true
		;;
	-n | --dryrun)
		flag_dryrun=true
		;;
	-a | --apply)
		flag_apply=true
		;;
	-*)
		echo "bad option $1"
		exit 1
		;;
	*)
		sfc_list+=($1)
		;;
	esac
	shift
done

if [ ${flag_dryrun} == true ] && [ ${flag_apply} == true ]; then
	echo "ERROR: Can't use --dryrun and --apply option simultaneously."
	exit 1
elif [ ${flag_dryrun} == false ] && [ ${flag_apply} == false ]; then
	flag_dryrun=true
fi

pushd ${sfc_path} >/dev/null

if [ ${flag_fullformat} == true ] || [ ${#sfc_list[@]} == 0 ]; then
	# The exception list for 'find' command should match with .githooks/pre-commit of sfc
	mapfile -d $'\0' list_files \
		< <(find ${sfc_path} '(' -path ${sfc_path}/build \
			-o -path ${sfc_path}/vsm_codegen \
			-o -path ${sfc_path}/compiler \
			-o -path ${sfc_path}/validator/build \
			-o -path ${sfc_path}/model/SFC/generated \
			-o -path ${sfc_path}/model/SFC/compiled \
			-o -path ${sfc_path}/model/ssfs/include/generated \
			-o -path ${sfc_path}/model/ssfs/src/generated \
			-o -path ${sfc_path}/*/generated ')' -prune \
			-o '(' -name '*.hpp' -o -name '*.cpp' -o -name '*.h' -o -name '*.c' ')' \
			-print0)
	list_files+=("${sfc_path}/vsm_codegen/template_vsmnode.hpp")
else
	list_dirs=()
	for sfc in "${sfc_list[@]}"; do
		mapfile -d $'\0' sfc_dirs < <(find ${sfc_path} -mindepth 5 -maxdepth 5 -type d -name "${sfc}.*" -print0)
		list_dirs=(${list_dirs[@]} ${sfc_dirs[@]})
	done
	[ ${#list_dirs[@]} -eq 0 ] && echo "No module found which the name is ${sfc_list[@]}" && exit 0
	list_files=()
	for dir in "${list_dirs[@]}"; do
		mapfile -d $'\0' -O "${#list_files[@]}" list_files \
			< <(find ${dir} '(' -path ${dir}/generated ')' -prune \
				-o '(' -name '*.hpp' -o -name '*.cpp' -o -name '*.h' -o -name '*.c' ')' \
				-print0)
	done
fi

[ ${#list_files[@]} -eq 0 ] && echo "There is no file to format." && exit 0

IFS=$'\n' list_files=($(sort <<<"${list_files[*]}" | uniq))
unset IFS

setterm -linewrap off
num_files=${#list_files[@]}
string_progress="Checking code formatting: "
echo -e -n "${string_progress}\r"
list_targets=()
for ((i = 0; i < ${num_files}; i++)); do
	percent=$(($((i + 1)) * 100 / ${num_files}))
	printf "\033[2K%s%3d%% (%${#num_files}d/%d), %s\r" \
		"${string_progress}" "${percent}" "$((i + 1))" "${num_files}" "${list_files[i]##*/}"
	if grep -q -e "// NOLINT.*" ${list_files[i]} &>/dev/null; then
		list_targets+=("${list_files[i]}")
	elif ! ${clang_format} --dry-run --Werror -i ${list_files[i]} &>/dev/null; then
		list_targets+=("${list_files[i]}")
	fi
done
printf "\033[2K%s%3d%% (%${#num_files}d/%d), done.\n" \
	"${string_progress}" 100 "${num_files}" "${num_files}"
setterm -linewrap on

if [ ${flag_dryrun} == true ]; then
	if [ ${#list_targets[@]} -eq 0 ]; then
		echo "OK. It looks like formatted well already for given file(s)."
	else
		echo "List of file(s) which needs to be formatted:"
		for file in "${list_targets[@]}"; do
			echo ${file#${sfc_path}/}
		done
		echo "Total: ${#list_targets[@]} file(s)"
	fi
elif [ ${flag_apply} == true ]; then
	if [ ${#list_targets[@]} -eq 0 ]; then
		echo "OK. It looks like formatted well already for given file(s)."
	else
		echo "Applying code formatting on ${#list_targets[@]} file(s)..."
		# [ ${#list_targets[@]} -ne 0 ] &&
		#     printf "%s\0" "${list_targets[@]}" |
		#     xargs -0 -i ${clang_format} --verbose -i '{}'
		for ((i = 0; i < ${#list_targets[@]}; i++)); do
			sed -e "s/\/\/ NOLINT.*//g" -i ${list_targets[i]}
			${clang_format} --verbose -i ${list_targets[i]}
		done
		echo "OK. Code formatting has completed for ${#list_targets[@]} file(s)."
	fi
fi

popd >/dev/null
