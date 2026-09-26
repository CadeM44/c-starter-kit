#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"  # self location
TEMPLATES_DIR="${SCRIPT_DIR}/templates"

usage() {
	cat >&2 <<'EOF'
Usage:
  cstarter.sh --name <project> --standard c99|c23 [--output <dir>]
  cstarter.sh <project>                 interactive standard picker
  cstarter.sh --list
  cstarter.sh --gui
  cstarter.sh --help

Standards:
  c99  C99 + CMake
  c23  C23 + CMake

Project names need to be C identifiers: [A-Za-z_][A-Za-z0-9_]*
EOF
}

list_standards() {
	printf '%s\n' c99 c23
}

is_ident() {
	[[ "$1" =~ ^[A-Za-z_][A-Za-z0-9_]*$ ]]
}

template_for_standard() {
	case "${1:-}" in
		c99|C99) printf '%s\n' "c99-starter" ;;
		c23|C23|c2x|C2X) printf '%s\n' "c2x-starter" ;;
		*)
			echo "Unknown standard: ${1:-} (c99 or c23)" >&2
			return 1
			;;
	esac
}

pick_standard_interactive() {
	echo "Please select a language standard"
	local x
	select x in c99 c23; do
		if [[ -n "${x}" ]]; then
			printf '%s\n' "${x}"
			return 0
		fi
	done
	return 1
}

apply_placeholders() {
	local project_dir="$1"
	local pname="$2"
	local list path dir base

	list="$(mktemp)"
	find "${project_dir}" -depth -name '*PROJECTNAME*' > "${list}"
	while IFS= read -r path; do
		[[ -z "${path}" ]] && continue
		dir="$(dirname "${path}")"
		base="$(basename "${path}")"
		mv "${path}" "${dir}/${base//PROJECTNAME/${pname}}"
	done < "${list}"
	rm -f "${list}"

	find "${project_dir}" -type f -exec sed -i "s/PROJECTNAME/${pname}/g" {} +
}

generate() {
	local pname="$1"
	local template="$2"
	local output_dir="$3"

	if [[ -z "${pname}" ]]; then
		echo "Project name is required" >&2
		return 1
	fi
	if ! is_ident "${pname}"; then
		echo "Invalid project name '${pname}'. Use a C identifier: [A-Za-z_][A-Za-z0-9_]*" >&2
		return 1
	fi
	if [[ ! -d "${TEMPLATES_DIR}" ]]; then
		echo "Unable to find template dir: ${TEMPLATES_DIR}" >&2
		return 2
	fi
	if [[ -z "${template}" ]]; then
		echo "Template is required" >&2
		return 1
	fi
	if [[ ! -d "${TEMPLATES_DIR}/${template}" ]]; then
		echo "Unknown template: ${template}" >&2
		echo "Available standards:" >&2
		list_standards >&2
		return 1
	fi

	mkdir -p "${output_dir}"
	local project_dir
	project_dir="$(cd "${output_dir}" && pwd)/${pname}"

	if [[ -e "${project_dir}" ]]; then
		echo "Project dir already exists: ${project_dir}" >&2
		return 3
	fi

	cp -R "${TEMPLATES_DIR}/${template}" "${project_dir}"
	apply_placeholders "${project_dir}" "${pname}"

	if [[ -f "${SCRIPT_DIR}/.editorconfig" ]]; then
		cp "${SCRIPT_DIR}/.editorconfig" "${project_dir}/"
	fi
	if [[ -f "${SCRIPT_DIR}/.clang-format" ]]; then
		cp "${SCRIPT_DIR}/.clang-format" "${project_dir}/"
	fi

	echo "Created ${project_dir}"
}

launch_gui() {
	local bin=""
	local candidate
	for candidate in \
		"${SCRIPT_DIR}/gui/cstarter-gui" \
		"${SCRIPT_DIR}/gui/build/cstarter-gui" \
		"${SCRIPT_DIR}/gui/build/Release/cstarter-gui" \
		"${SCRIPT_DIR}/gui/build/Debug/cstarter-gui"
	do
		if [[ -x "${candidate}" ]]; then
			bin="${candidate}"
			break
		fi
	done

	if [[ -z "${bin}" && -f "${SCRIPT_DIR}/gui/main.c" ]]; then
		echo "Clay GUI isn't built yet." >&2
		if command -v cmake >/dev/null 2>&1; then
			echo "Building it with CMake (fetches raylib if needed)..." >&2
			cmake -S "${SCRIPT_DIR}/gui" -B "${SCRIPT_DIR}/gui/build" && \
				cmake --build "${SCRIPT_DIR}/gui/build"
			if [[ -x "${SCRIPT_DIR}/gui/build/cstarter-gui" ]]; then
				bin="${SCRIPT_DIR}/gui/build/cstarter-gui"
			fi
		elif command -v make >/dev/null 2>&1 && pkg-config --exists raylib 2>/dev/null; then
			echo "Building it with make..." >&2
			make -C "${SCRIPT_DIR}/gui"
			if [[ -x "${SCRIPT_DIR}/gui/cstarter-gui" ]]; then
				bin="${SCRIPT_DIR}/gui/cstarter-gui"
			fi
		fi
	fi

	if [[ -n "${bin}" ]]; then
		export CSTARTER_ROOT="${SCRIPT_DIR}"
		exec "${bin}"
	fi

	echo "Native Clay GUI isn't available." >&2
	echo "Build it with: cmake -S gui -B gui/build && cmake --build gui/build" >&2
	return 1
}

NAME=""
STANDARD=""
OUTPUT="."
DO_LIST=0
DO_GUI=0

while [[ $# -gt 0 ]]; do
	case "$1" in
		-h|--help)
			usage
			exit 0
			;;
		-l|--list)
			DO_LIST=1
			shift
			;;
		-g|--gui)
			DO_GUI=1
			shift
			;;
		-n|--name)
			NAME="${2:-}"
			shift 2
			;;
		-s|--standard)
			STANDARD="${2:-}"
			shift 2
			;;
		-o|--output)
			OUTPUT="${2:-}"
			shift 2
			;;
		--)
			shift
			break
			;;
		-*)
			echo "Unknown option: $1" >&2
			usage
			exit 1
			;;
		*)
			if [[ -z "${NAME}" ]]; then
				NAME="$1"
			else
				echo "Unexpected argument: $1" >&2
				usage
				exit 1
			fi
			shift
			;;
	esac
done

if [[ "${DO_LIST}" -eq 1 ]]; then
	list_standards
	exit 0
fi

if [[ "${DO_GUI}" -eq 1 ]]; then
	launch_gui
	exit 0
fi

if [[ -z "${NAME}" ]]; then
	usage
	exit 1
fi

if [[ -z "${STANDARD}" ]]; then
	STANDARD="$(pick_standard_interactive)" || exit 1
fi

TEMPLATE="$(template_for_standard "${STANDARD}")" || exit 1
generate "${NAME}" "${TEMPLATE}" "${OUTPUT}"
