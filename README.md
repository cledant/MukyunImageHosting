# Mukyun Image Hosting  

## Feature  
Simple Rest API that saves `png`, `jpeg`, `gif`, `webp` images  

## Compile  
Make sure you have all libraries by running `git submodule init && git submodule update`.  
You may compile `mukyun_image_hosting` binary by running `cmake`.  

## Usage  
Generate `Mukyun Image Hosting` config file by running `mukyun_image_hosting --generate_default_cfg`.  
Edit that file and set its path in `MUKYUN_IH_CFG_PATH` environment variable.  
Run `mukyun_image_hosting`.

## Additional requirement  
You need a reverse proxy such as `nginx` to handle file delivery and HTTPS.  

## Upload images  
First edit `mukyun_ih_upload.sh` to set your token and your server address.  
Then run `mukyun_ih_upload.sh image` to upload your image. 