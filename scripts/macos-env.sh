#!/bin/bash

# TODO:Change paths after adding vulkan sdk directly into project
export GAME_ENV_ON=1

export DYLD_LIBRARY_PATH=$HOME/programming/vulkan/macOS/lib
export VK_ICD_FILENAMES=$HOME/programming/vulkan/macOS/share/vulkan/icd.d/MoltenVK_icd.json
export VK_LAYER_PATH=$HOME/programming/vulkan/macOS/share/vulkan/explicit_layer.d

export PATH=$PATH:/$HOME/programming/vulkan/macOS/bin/


alias game_debug="lldb bin/game -v DYLD_LIBRARY_PATH=/Users/brinq/programming/vulkan/macOS/lib -v VK_ICD_FILENAMES=/Users/brinq/programming/vulkan/macOS/share/vulkan/icd.d/MoltenVK_icd.json -v VK_LAYER_PATH=/Users/brinq/programming/vulkan/macOS/share/vulkan/explicit_layer.d"
