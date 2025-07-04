#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_REZ=""

options=(-i -v -c -l -n -h -s -o)

declare -a one_f_w_m_tests=(
"VAR ly grep_1.txt"
)

declare -a one_f_wout_m_tests=(
"VAR grog grep_1.txt"
)

declare -a two_f_w_m_tests=(
"VAR ly grep_1.txt grep_2.txt"
)

declare -a two_f_wout_m_tests=(
"VAR grog grep_1.txt grep_2.txt"
)

declare -a two_f_w_one_m_tests=(
"VAR ness grep_1.txt grep_2.txt"
)

declare -a flag_f_tests=(
"VAR grep_1.txt grep_2.txt"
)

if [[ ! -s ./s21_grep ]]; then
    echo "Помести test_for_grep.sh в папку с исполняемым файлом s21_grep"
    exit
fi

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_grep $t > s21_result
    grep $t > grep_result
    DIFF_REZ="$(diff -s grep_result s21_result)"
    (( COUNTER++ ))
    if [ "$DIFF_REZ" == "Files grep_result and s21_result are identical" ]
    then
      (( SUCCESS++ ))
        echo "\tSUCCESS grep $t"
    else
      (( FAIL++ ))
        echo "\tFAIL grep $t"
    fi
    rm s21_result grep_result
}

echo "\n\n\n\n\n TESTING with ONE file WITH match \n\n\n\n\n"

for var1 in ${options[@]}
do
    for var2 in ${options[@]}
    do
        for var3 in ${options[@]}
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${one_f_w_m_tests[@]}"
                do
                    var="$var1 $var2 $var3"
                    testing $i
                done
            fi
        done
    done
done

echo "\n\n\n\n\n TESTING with ONE file WITHOUT match \n\n\n\n\n"

for var1 in ${options[@]}
do
    for var2 in ${options[@]}
    do
        for var3 in ${options[@]}
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${one_f_wout_m_tests[@]}"
                do
                    var="$var1 $var2 $var3"
                    testing $i
                done
            fi
        done
    done
done

echo "\n\n\n\n\n TESTING with TWO files WITH match \n\n\n\n\n"

for var1 in ${options[@]}
do
    for i in "${two_f_w_m_tests[@]}"
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
            for i in "${two_f_w_m_tests[@]}"
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
                for i in "${two_f_w_m_tests[@]}"
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
                    for i in "${two_f_w_m_tests[@]}"
                    do
                        var="$var1 $var2 $var3 $var4"
                        testing $i
                    done
                fi
            done
        done
    done
done

echo "\n\n\n\n\n TESTING with TWO files WITHOUT match \n\n\n\n\n"

for var1 in ${options[@]}
do
    for var2 in ${options[@]}
    do
        for var3 in ${options[@]}
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${two_f_wout_m_tests[@]}"
                do
                    var="$var1 $var2 $var3"
                    testing $i
                done
            fi
        done
    done
done

echo "\n\n\n\n\n TESTING with TWO files WITH ONE match \n\n\n\n\n"

for var1 in ${options[@]}
do
    for var2 in ${options[@]}
    do
        for var3 in ${options[@]}
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${two_f_w_one_m_tests[@]}"
                do
                    var="$var1 $var2 $var3"
                    testing $i
                done
            fi
        done
    done
done

echo "\n\n\n\n\n TESTING FLAG -e \n\n\n\n\n"

for i in "${two_f_w_m_tests[@]}"
do
    var="-e"
    testing $i
done

for i in "${two_f_w_m_tests[@]}"
do
    var="-e bam -e"
    testing $i
done

for var1 in ${options[@]}
do
    for var2 in ${options[@]}
    do
        for var3 in ${options[@]}
        do
            if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
            then
                for i in "${two_f_w_m_tests[@]}"
                do
                    var="-e bam $var1 $var2 $var3 -e"
                    testing $i
                done
            fi
        done
    done
done

echo "\n\n\n\n\n TESTING FLAG -f \n\n\n\n\n"

for i in "${flag_f_tests[@]}"
do
    var="-f grep_f1.txt"
    testing $i
done

for i in "${flag_f_tests[@]}"
do
    var="-f grep_f1.txt -f grep_f2.txt"
    testing $i
done

for var1 in ${options[@]}
do
    for i in "${flag_f_tests[@]}"
    do
        var="$var1 -f grep_f1.txt"
        testing $i
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
                for i in "${flag_f_tests[@]}"
                do
                    var="-f grep_f1.txt $var1 $var2 $var3 -f grep_f2.txt"
                    testing $i
                done
            fi
        done
    done
done

echo "FAIL: $FAIL"
echo "SUCCESS: $SUCCESS"
echo "ALL: $COUNTER"
