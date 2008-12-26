#!/bin/bash

# psxc-trailer v0.2.2008.12.26
##############################
#
# Small script that fetches the qt trailer and image for movies.
# Takes one argument (path to releasedir). If no arg is given, it uses
# current path.
#
# Required bins are wget, sed, echo, tr, cut, head, tail, grep, bash, wc
#
# Use as a site command:
#   Make sure all required bins are availible in chroot.
#   Add the following to glftpd.conf
#     site_cmd		TRAILER	EXEC	/bin/psxc-trailer.sh
#     custom-trailer	1 2 7
#
# Use as a nfo-script in pzs-ng:
#   Not really recommended as it may take a few seconds for the script to
#   download the trailer.
#
# Can probably also be used with total-rescan and other scripts, but this is
# untested.

#
################# CONFIG OPTIONS #################
#
# What dirs to execute in (no wildcards). Use "/" to include all.
validdirs="/site/MOVIES/ /site/FILMS/"

# quality of trailer. Choose between
# 320 (smallest), 480, 640, 480p, 720p, 1080p (highest)
# More than one quality setting is allowed - first found will be used
# use "" to disable
trailerquality="480p 640 480 320"

# what name should be used on the trailer?
# use "" to keep name as is.
trailername="trailer.mov"

# download trailer image? ("yes"=yes, ""=no)
downloadimage="yes"

# if yes, what name is to be used?
imagename=folder.jpg

# you can define how accurate you wish the search to be. lower the number
# if you need more results, or increase if you get a lot of false positives
accuracy=1

#
###### ADVANCED CONFIG - USUALLY NO NEED TO CHANGE THESE VARS ######
#
# words to ignore/remove from search - case does not matter
removewords="XViD DiVX H264 x264 DVDR 720p 1080p BluRay BluRay BRRiP HDTV CAM TC TELECINE TS TELESYNC SCREENER R5 SCR DVDSCR DVDSCREENER DVDRiP REPACK PROPER INTERNAL LiMiTED UNRATED READNFO LINE DiRECT SYNCFIX AC3 DTS DTS-ES"

# wget output - usually set to /dev/null but can be directed to a file for debug purposes
wgetoutput=/dev/null

# wget tempfile - usually no need to change
wgettemp=the.fake

# make sure we have access to all bins needed. Should not need to change this
PATH=$PATH:/bin:/usr/bin:/usr/local/bin:/sbin:/usr/sbin:/usr/local/sbin:/glftpd/bin:$HOME/bin

# debug option. do not remove the hash unless you know what you're doing
#set -x -v

######################################## END OF CONFIG ##########################################

# code below
[[ "$1" != "" && -d "$1" ]] && {
  cd $1
}
founddir=""
for validdir in $validdirs; do
  [[ "$(echo $PWD | grep $validdir)" != "" ]] && {
    founddir="yes"
    break
  }
done
[[ "$founddir" == "" ]] && {
  echo "Not in a moviedir - not searching for a trailer"
  exit 0
}
count=$(echo $PWD | tr -cd '-' | wc -c)
let count=count+0
[[ count -ge 1 ]] && {
  releasename="$(echo "$PWD" | tr '/' '\n' | tail -n 1 | cut -d '-' -f 1-$count | tr 'A-Z' 'a-z')"
} || {
  releasename="$(echo "$PWD" | tr '/' '\n' | tail -n 1 | tr 'A-Z' 'a-z')"
}
while [ 1 ]; do
  whilename=$releasename
  for word in $removewords; do
    word=$(echo $word | tr 'A-Z' 'a-z')
    rname="$(echo "$releasename" | sed -E "s/[\.|_]$word$//")"
    [[ "$releasename" != "$rname" ]] && {
      releasename=$rname
      break
    }
  done
  [[ "$releasename" == "$whilename" ]] && {
    break
  }
done

echo "Trying to download trailer for \"$(echo $releasename | tr '\.' ' ')\"..."
orgrelname=$releasename
countdot=$(echo $releasename | tr -cd '\.' | wc -c)
let countdot=countdot-0

while [ 1 ]; do
  releasename="$(echo "$releasename" | sed -E "s/[\.|_][12][0-9][0-9][0-9]$//" | tr -d '\.')"
  output="$(wget --ignore-length --timeout=10 -o $wgetoutput -O - "http://www.apple.com/trailers/home/scripts/quickfind.php?q=$releasename")"
  outparse="$(echo $output | tr -d '\"' | tr ',' '\n')"
  iserror=$(echo $outparse | grep -i "error:true")
  isresult=$(echo $outparse | grep -i "results:\[\]")
  [[ "$iserror" != "" ]] && {
    echo "An error occured. Unable to parse output"
    exit 1
  }
  [[ "$isresult" == "" ]] && {
    break
  }
  [[ $countdot -le $accuracy ]] && {
    break
  }
  releasename=$(echo $orgrelname | cut -d '.' -f 1-$countdot)
  let countdot=countdot-1
  [[ $countdot -lt 1 ]] && {
    break
  }
done
[[ "$isresult" != "" ]] && {
  echo "Unable to find movie"
  exit 0
}

poster="$(echo "$outparse" | grep -i "^poster:" | cut -d ':' -f 2- | tr -d '\\')"
location="http://www.apple.com$(echo "$outparse" | grep -i "^location:" | cut -d ':' -f 2- | tr -d '\\' | tr ' ' '\n' | head -n 1)"

output2="$(wget --ignore-length --timeout=10 -o $wgetoutput -O - $location)"
output2parse="$(echo $output2 | tr ' \?' '\n' | grep -E -i "^href=.*\.mov[\"]*$|^href=.*small[_]?.*\.html[\"]*|^href=.*medium[_]?.*\.html[\"]*|^href=.*large[_]?.*\.html[\"]*|^href=.*low[_]?.*\.html[\"]*|^href=.*high[_]?.*\.html[\"]?.*" | tr -d '\"' | cut -d '=' -f 2-)"
#echo DEBUG 1: $output2parse
#echo DEBUG 5: $location
for quality in $trailerquality; do
  urllink=$(echo "$output2parse" | grep -i "${quality}.mov$" | head -n 1)
  [[ "$urllink" != "" ]] && {
    break
  }
  sublink=""
  [[ "$quality" == "320" && "$(echo "$output2parse" | tr ' ' '\n'  | grep -E -i "small[_]?.*.html|low[_]?.*.html")" != "" ]] && {
    sublink=${location}$(echo "$output2parse" | grep -E -i "small.html[_]?.*|low[_]?.*.html" | tr '\>\<\ ' '\n' | head -n 1)
    movsearch="\.mov\$"
  }
  [[ "$quality" == "480" && "$(echo "$output2parse" | tr ' ' '\n' | grep -E -i "medium[_]?.*.html")" != "" ]] && {
    sublink=${location}$(echo "$output2parse" | grep -E -i "medium[_]?.*.html" | tr '\>\<\ ' '\n' | head -n 1)
    movsearch="\.mov\$"
  }
  [[ "$quality" == "640" && "$(echo "$output2parse" | tr ' ' '\n'  | grep -E -i "large[_]?.*.html|high[_]?.*.html")" != "" ]] && {
    sublink=${location}$(echo "$output2parse" | grep -E -i "large[_]?.*.html|high[_]?.*.html" | tr '\>\<\ ' '\n' | head -n 1)
    movsearch="\.mov\$"
  }
  [[ "$quality" == "480p" || "$quality" == "720p" || "$quality" == "1080p" ]] && {
    sublink="${location}hd/"
    movsearch="${quality}.mov\$"
  }
  #echo DEBUG 4: $sublink
  [[ "$sublink" != "" ]] && {
    output3="$(wget --ignore-length --timeout=10 -o $wgetoutput -O - $sublink)"
    output3parse="$(echo $output3 | tr -c 'a-zA-Z/:\._0-9\-' '\n' | grep -E -i "$movsearch")"
    urllink=$(echo "$output3parse" | grep -i "\.mov$" | head -n 1)
    #echo DEBUG 2: $output3parse
  }
  [[ "$urllink" != "" ]] && {
    break
  }
done
#echo DEBUG 3: $urllink
[[ "$urllink" == "" ]] && {
  echo "Failed to fetch movielink"
  exit 0
}

# download trailer and picture
[[ "$downloadimage" != "" && "$poster" != "" ]] && {
  echo "Downloading posterimage as $imagename"
  wget --ignore-length --timeout=10 -o $wgetoutput -O $imagename $poster
}

[[ "$trailerquality" != "" && "$urllink" != "" ]] && {
  wget --ignore-length --timeout=10 -o $wgetoutput -O $wgettemp $urllink
  fakelinkname=$(echo $urllink | tr '/' '\n' | grep -i "\.mov$")
  reallinkname=$(cat $wgettemp | tr -c 'a-zA-Z0-9\-\.\_' '\n' | grep -i "\.mov")
  reallink=$(echo $urllink | sed "s|$fakelinkname|$reallinkname|")
  rm -f $wgettemp
  [[ "$trailername" == "" ]] && {
    trailername=$(echo $urllink | tr '/' '\n' | grep -i "mov$" | tail -n 1)
  }
  echo "Downloading trailer in $quality quality as $trailername"
  wget --ignore-length --timeout=10 -o $wgetoutput -O $trailername $reallink
}
echo "done"
exit 0
