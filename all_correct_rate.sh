for t in 5000 10000 15000 20000 25000;do
    echo "t ${t}"
    for s in result_v_2_d_*;do
	cd $s;
	echo "$s  "
	
	for f in p_*.txt;do
	    cat $f|\
		grep train_correct_late_t_$t ;
	done | awk '{sum+=$2}END{printf("train_correct\t%s\n",sum/NR);}'

	for f in p_*.txt;do
	    cat $f|\
		grep train_TP_t_$t ;
	done | awk '{sum+=$2}END{printf("train_TP\t%s\n",sum/NR);}'
	for f in p_*.txt;do
	    cat $f|\
		grep train_FN_t_$t ;
	done | awk '{sum+=$2}END{printf("train_FN\t%s\n",sum/NR);}'
	for f in p_*.txt;do
	    cat $f|\
		grep train_FP_t_$t ;
	done | awk '{sum+=$2}END{printf("train_FP\t%s\n",sum/NR);}'
	for f in p_*.txt;do
	    cat $f|\
		grep train_TN_t_$t ;
	done | awk '{sum+=$2}END{printf("train_TN\t%s\n\n",sum/NR);}'


	
	for f in p_*.txt;do
	    cat $f|\
		grep test_correct_late_t_$t ;
	done | awk '{sum+=$2}END{printf("test_correct\t%s\n",sum/NR);}'
    
	for f in p_*.txt;do
	    cat $f|\
		grep test_TP_t_$t ;
	done | awk '{sum+=$2}END{printf("test_TP\t%s\n",sum/NR);}'
	for f in p_*.txt;do
	    cat $f|\
		grep test_FN_t_$t ;
	done | awk '{sum+=$2}END{printf("test_FN\t%s\n",sum/NR);}'
	for f in p_*.txt;do
	    cat $f|\
		grep test_FP_t_$t ;
	done | awk '{sum+=$2}END{printf("test_FP\t%s\n",sum/NR);}'
	for f in p_*.txt;do
	    cat $f|\
		grep test_TN_t_$t ;
	done | awk '{sum+=$2}END{printf("test_TN\t%s\n",sum/NR);}'
	
	cd ..;
    done | tee result_t_${t}.txt
done



