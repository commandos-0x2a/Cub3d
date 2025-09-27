

ffmpeg -f lavfi -i color=c=white:s=64x64 \
 -vf "drawtext=text='$1':x=(w-text_w)/2:y=(h-text_h)/2:fontsize=200:fontcolor=black" \
 -frames:v 1 -f rawvideo -pix_fmt rgb24 $2/$1.rgb
