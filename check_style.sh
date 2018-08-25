#!/bin/zsh

for f in $(git ls-tree -r --name-only HEAD); do
    args=$(grep cpplint: ${f} | awk '{ print $3 }')
    cpplint ${args} ${f};
done
