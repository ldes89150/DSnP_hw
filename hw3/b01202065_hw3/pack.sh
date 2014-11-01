#!/bin/bash
STUDENTID="b01202065";
HOMEWORK_NUM="_hw3/";
echo "Packing Homework "${STUDENTID}${HOMEWORK_NUM};
if [ ! -d ${STUDENTID}${HOMEWORK_NUM} ]; then
echo "Creating Directory "${STUDENTID}${HOMEWORK_NUM};
mkdir ${STUDENTID}${HOMEWORK_NUM};
else
while true; do 
read -p "Directory already exist! Continuing could distroy all previous
files in derectory $STUDENTID${HOMEWORK_NUM}! Do you want to continue? (Y/n)" ans
case $ans in
[Yy]* ) break;;
[Nn]* ) echo "Terminating packing program"; exit;;
* ) echo "Please reply yes or no";;
esac
done
fi
echo -e "Copying files...";
for line in $(cat ./MustExist.txt)
do
line=${line#$HOMEWORK_NUM};
if [ -e ${STUDENTID}${HOMEWORK_NUM}${line} ] ; then
echo -e "\nWarning! Overriding previous existence of
"${STUDENTID}${HOMEWORK_NUM}$line;
fi
if [ ! -e ${line} ]; then
echo -e "\nWarning! $line doesn't exist!";
else 
cp --parents $line ${STUDENTID}${HOMEWORK_NUM};
fi
done
echo -e "\nStripping must remove files...";
for line in $(cat ./MustRemove.txt)
do
if [ -e ${STUDENTID}${line} ] ; then
echo "Removing redudent file $STUDENTID$line...";
rm ${STUDENTID}${line};
fi
done
echo "Compressing file";
tar -zcf "${STUDENTID}${HOMEWORK_NUM%"/"}.tar.gz" "${STUDENTID}${HOMEWORK_NUM}";
echo "Cleaning up";
rm -r ${STUDENTID}${HOMEWORK_NUM}

./SelfCheck "${STUDENTID}${HOMEWORK_NUM%"/"}.tar.gz"

