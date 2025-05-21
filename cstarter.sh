#!/bin/bash

usage() {
    >&2 echo "Usage: $0 <project>"
    return 0
}

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"  #“self location” trick in Bash to find the directory where the script itself lives
templatesDir="$SCRIPT_DIR/templates"

pname=$1
projectDir="./${pname}"


if [ -z "$pname" ]; then
    usage
    exit 1
elif ! [ -d "$templatesDir" ]; then
    >&2 echo "Unable to find template dir: $templatesDir"
    exit 2
elif [ -d "$projectDir" ]; then
    >&2 echo "Project dir already exists: $projectDir"
    exit 3
fi

cur="$PWD"
cd $templatesDir
echo "Please select a template"
select x in *; do
    template="$x"
    break
done

cd $cur
cp -R ${templatesDir}/$template $projectDir
cd $projectDir

if [[ "$template" == "c2x-birch-starter" ]]; then
  for x in *; do
    new=$(sed "s,PROJECTNAME,$pname,g" <<< "$x")
    if [ "$x" = "$new" ]; then
      sed "s,PROJECTNAME,$pname,g" < "$x" > temp
      mv -f temp "$x"
    else
      sed "s,PROJECTNAME,$pname,g" < "$x" > "$new"
      rm -f "$x"
    fi
  done

else
  # Recursive find-based loop
  # Rename all files & dirs
  find . -depth -name '*PROJECTNAME*' | while IFS= read -r path; do
    dir=$(dirname "$path")
    base=$(basename "$path")
    mv "$path" "$dir/${base//PROJECTNAME/$pname}"
  done

  # Replace inside every file
  find . -type f -exec sed -i "s/PROJECTNAME/$pname/g" {} +

fi

cp "$SCRIPT_DIR/.editorconfig" ./
cp "$SCRIPT_DIR/.clang-format" ./