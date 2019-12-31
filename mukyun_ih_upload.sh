#!/bin/sh

#Config
server_addr=127.0.0.10
server_port=4242
api_key=Patchouli

#Parsing file type
file=$1
file_type=${file##*.}
if [[ "$file_type" == "jpg" ]]; then
    file_type=jpeg
fi

#Sending image
curl -X POST --data-binary "@${file}" -H "Content-Type: image/${file_type}" -H \
"x-mukyun-ih: ${api_key}" -H "Expect:" ${server_addr}:${server_port}/saveimage
