make
for file in `ls tests`; do
echo "**************************************************************************************"
echo -e "Testing tests/\c"
echo $file
./main $file
done