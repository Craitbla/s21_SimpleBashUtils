#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_REZ=""

options=(-b -e -n -s -t -v)

declare -a tests=(
"VAR cat_1.txt cat_2.txt"
)

if [[ ! -s ./s21_cat ]]; then
    echo "Помести test_for_cat.sh в папку с исполняемым файлом s21_cat"
    exit
fi

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_cat $t > s21_result
    cat $t > cat_result
    DIFF_REZ="$(diff -s cat_result s21_result)"
    (( COUNTER++ ))
    if [ "$DIFF_REZ" == "Files cat_result and s21_result are identical" ]
    then
      (( SUCCESS++ ))
        echo "\tSUCCESS cat $t"
    else
      (( FAIL++ ))
        echo "\tFAIL cat $t"
    fi
    rm s21_result cat_result
}

for var1 in ${options[@]}
do
    for i in "${tests[@]}"
    do
        var="$var1"
        testing $i
    done
done

for var1 in ${options[@]}
do
    for var2 in ${options[@]}
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="$var1 $var2"
                testing $i
            done
        fi
    done
done

for var1 in ${options[@]}
do
    for var2 in ${options[@]}
    do
        for var3 in ${options[@]}
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${tests[@]}"
                do
                    var="$var1 $var2 $var3"
                    testing $i
                done
            fi
        done
    done
done

for var1 in ${options[@]}
do
    for var2 in ${options[@]}
    do
        for var3 in ${options[@]}
        do
            for var4 in ${options[@]}
            do
                if [ $var1 != $var2 ] && [ $var2 != $var3 ] \
                && [ $var1 != $var3 ] && [ $var1 != $var4 ] \
                && [ $var2 != $var4 ] && [ $var3 != $var4 ]
                then
                    for i in "${tests[@]}"
                    do
                        var="$var1 $var2 $var3 $var4"
                        testing $i
                    done
                fi
            done
        done
    done
done
echo "FAIL: $FAIL"
echo "SUCCESS: $SUCCESS"
echo "ALL: $COUNTER"
