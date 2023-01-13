param ($param1)
write-host $param1
git add . -A
git commit -m"$param1"
# git tag $tag
git push
# git push --tags