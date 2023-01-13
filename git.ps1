param ($tag)
write-host $tag
git add . -A
git commit -m"$tag"
git tag $tag
git push
git push --tags
$from = "my"
$to = "tags/$tag"
Copy-Item -Path (Get-Item -Path "$from\*" -Exclude ('.pio')).FullName -Destination $to -Recurse -Force
