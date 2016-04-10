#!/bin/bash

shopt -s nullglob

for dirs in */
do
	cd $dirs

	for file in *.vert
	do
	  echo "Building vertex shader '$file'"
	  glslangValidator -V $file | grep "ERROR"
	  mv vert.spv ../../SPIR/V/$file.spv &> /dev/null
	  echo
	done

	for file in *.tesc
	do
	  echo "Building tessellation control shader '$file'"
	  glslangValidator -V $file | grep "ERROR"
	  mv tesc.spv ../../SPIR/V/$file.spv &> /dev/null
	  echo
	done

	for file in *.tese
	do
	  echo "Building tessellation evaluation shader '$file'"
	  glslangValidator -V $file | grep "ERROR"
	  mv tese.spv ../../SPIR/V/$file.spv &> /dev/null
	  echo
	done

	for file in *.geom
	do
	  echo "Building geometry shader '$file'"
	  glslangValidator -V $file | grep "ERROR"
	  mv geom.spv ../../SPIR/V/$file.spv &> /dev/null
	  echo
	done

	for file in *.frag
	do
	  echo "Building fragment shader '$file'"
	  glslangValidator -V $file | grep "ERROR"
	  mv frag.spv ../../SPIR/V/$file.spv &> /dev/null
	  echo
	done

	for file in *.comp
	do
	  echo "Building compute shader '$file'"
	  glslangValidator -V $file | grep "ERROR"
	  mv comp.spv ../../SPIR/V/$file.spv &> /dev/null
	  echo
	done

	cd ..
done

