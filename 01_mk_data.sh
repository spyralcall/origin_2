#再現性なし
#クレームを抜き出してシャッフルしてテキストに書き出し
grep -w "001" claim_data.txt | grep -v "^#" | shuf > shuf_cl_claim_data.txt
#相談・問い合わせを抜き出してシャッフルしてテキストに書き出し
grep -w "003" claim_data.txt | grep -v "^#" | shuf > shuf_soudan_claim_data.txt

#クレームを約十分割したデータを作成(1-6は142個,7-10は141個)
for c in {1..6};do
    #ヘッダの書き込み
    head -n 1 claim_data.txt > 10_fold_claim_test_data_${c}.txt
    head -n 1 claim_data.txt > 10_fold_claim_train_data_${c}.txt
    #データの分割
    cat shuf_cl_claim_data.txt | sed -n $(((1+(${c}-1)*142))),$(((142*${c})))p >> 10_fold_claim_test_data_${c}.txt
    cat shuf_cl_claim_data.txt | sed -e $(((1+(${c}-1)*142))),$(((142*${c})))d >> 10_fold_claim_train_data_${c}.txt
done
for c in {7..10};do
    #ヘッダの書き込み
    head -n 1 claim_data.txt > 10_fold_claim_test_data_${c}.txt
    head -n 1 claim_data.txt > 10_fold_claim_train_data_${c}.txt
    #データの分割
    cat shuf_cl_claim_data.txt | sed -n $(((853+(${c}-7)*141))),$(((6+(141*c))))p >> 10_fold_claim_test_data_${c}.txt
    cat shuf_cl_claim_data.txt | sed -e $(((853+(${c}-7)*141))),$(((6+(141*c))))d >> 10_fold_claim_train_data_${c}.txt
done




#相談問い合わせを約十分割したデータを作成(1-3は839個,4-10は840個)
for c in {1..3};do
    cat shuf_soudan_claim_data.txt | sed -n $(((1+(${c}-1)*839))),$(((839*${c})))p >> 10_fold_claim_test_data_${c}.txt
    cat shuf_soudan_claim_data.txt | sed -e $(((1+(${c}-1)*839))),$(((839*${c})))d >> 10_fold_claim_train_data_${c}.txt
    #番号順に並べる
    sort -nk1 10_fold_claim_test_data_${c}.txt > 10_fold_claim_test_${c}.txt
    sort -nk1 10_fold_claim_train_data_${c}.txt > 10_fold_claim_train_${c}.txt
    #sortする前のデータを削除
    rm 10_fold_claim_test_data_${c}.txt
    rm 10_fold_claim_train_data_${c}.txt
done

for c in {4..10};do
    cat shuf_soudan_claim_data.txt | sed -n $(((2518+(${c}-4)*840))),$((((840*${c})-3)))p >> 10_fold_claim_test_data_${c}.txt
    cat shuf_soudan_claim_data.txt | sed -e $(((2518+(${c}-4)*840))),$((((840*${c})-3)))d >> 10_fold_claim_train_data_${c}.txt
    #番号順に並べる
    sort -nk1 10_fold_claim_test_data_${c}.txt > 10_fold_claim_test_${c}.txt
    sort -nk1 10_fold_claim_train_data_${c}.txt > 10_fold_claim_train_${c}.txt
    #sortする前の作業データを削除
    rm 10_fold_claim_test_data_${c}.txt
    rm 10_fold_claim_train_data_${c}.txt
done







