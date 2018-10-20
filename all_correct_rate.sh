for t in 10000 20000;do
    echo "t ${t}"
    for s in result_v_2_d_*;do
	cd $s;
	echo -n "$s  "
	for f in p_*.txt;do
	    cat $f|\
		grep train_correct_late_t_$t ;
	done | awk '{sum+=$2}END{printf("%s\n",sum/NR);}'
	cd ..;
    done | tee train_result_t_${t}.txt
done



for t in 10000 20000;do
    echo "t ${t}"
    for s in result_v_2_d_*;do
	cd $s;
	echo -n "$s  "
	for f in p_*.txt;do
	    cat $f|\
		grep test_correct_late_t_$t ;
	done | awk '{sum+=$2}END{printf("%s\n",sum/NR);}'
	cd ..;
    done | tee test_result_t_${t}.txt
done
