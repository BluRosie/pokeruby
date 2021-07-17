for srcfile in data/tilesets/*.inc; do
  for file in $(tools/scaninc/scaninc -I include $srcfile); do
    if ! [ -f "${file}" ]; then
      make "${file}" -j12 || echo ${file}
    fi
  done
done
