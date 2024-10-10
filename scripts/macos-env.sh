#!/bin/bash

# TODO:Change paths after adding vulkan sdk directly into project

export DYLD_LIBRARY_PATH=$HOME/programming/vulkan/macOS/lib
export VK_ICD_FILENAMES=$HOME/programming/vulkan/macOS/share/vulkan/icd.d/MoltenVK_icd.json
export VK_LAYER_PATH=$HOME/programming/vulkan/macOS/share/vulkan/explicit_layer.d
