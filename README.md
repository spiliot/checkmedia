# checkmedia

A tiny linux program that let's you know if an optical drive has media loaded

## Compile
Just use gcc
`gcc checkmedia.c -o checkmedia`

And don't forget to set the executable bit
`chmod +x ./checkmedia`

## Run
`checkmedia /dev/sr0`

Assuming `/dev/sr0` is an optical drive of course.

## License
Freeware, public domain
