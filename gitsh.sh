#!/bin/bash
tag=$1
git add . -A
git commit -m"$tag"
git tag $tag
git push
git push --tags