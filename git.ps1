param ($tag)
write-host $tag
git add . -A
git commit -m"$tag"
git tag $tag
git push
git push --tags