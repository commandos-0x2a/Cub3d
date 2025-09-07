

ffmpeg \
 -f lavfi \
 -i color=c=white:s=480x480 \
 -vf "drawtext=text='$1':fontcolor=black:fontsize=200:x=(w-text_w)/2:y=(h-text_h)/2" \
 -frames:v 1 \
 -y $1.png
