for v in 2 3 4 5 6 7 8 9 10 ; do  #入力変数の数
    for d in 2 5 10 20 30 100 1000 10000 ; do  #中間層素子数の数
	for a in  0.2 0.4 0.6 0.8 ;do #学習率
	    for h in 0.5 ;do #出力閾値
		for t in 25000 ;do  #トレーニング回数
		    for r in  0.0001 0.00001 0.000001 ;do #正則化パラメータ
			mkdir result_v_${v}_d_${d}_a_${a}_last_theta_${h}_t_${t}_seisoku_${r}

			#学習データとテストデータの数がそれぞれ違うので場合分け、input_Dimは30固定、入力変数の数はvで指定
			#pattern 1
			for p in 1 2 3 7 8 9 10; do
			    for s in {1..5}; do
				echo " v ${v}  d ${d} a ${a} h ${h} p ${p} s ${s} r ${r}" 
				./read_sample_3_2 input_10_fold_claim_train_${p}.txt input_10_fold_claim_test_${p}.txt 8832 981 30 $d $a $t $s $v $h $r | \
				    tee result_v_${v}_d_${d}_a_${a}_last_theta_${h}_t_${t}_seisoku_${r}/p_${p}_s_${s}.txt
			    done
			done

			#pattern 2
			for p in 4 5 6; do
			    for s in {1..5}; do
				echo " v ${v}  d ${d} a ${a} h ${h} p ${p} s ${s} r ${r}" 
				./read_sample_3_2 input_10_fold_claim_train_${p}.txt input_10_fold_claim_test_${p}.txt 8831 982 30 $d $a $t $s $v $h $r | \
				    tee result_v_${v}_d_${d}_a_${a}_last_theta_${h}_t_${t}_seisoku_${r}/p_${p}_s_${s}.txt
			    done
			done

			
		    done		
		done
	    done
	done
    done
done    


