#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BUF_SIZE 2048

/* double型の二次元配列を確保する関数 */
double **alloc_2d_double(int m, int n);
/* double型の一次元配列を確保する関数 */
double *alloc_1d_double(int m);
/*中間層の出力計算をする関数y_calの宣言*/
double y_cal(double *v, double theta_h, double *xtrain, int Dim);
/*出力層の出力計算をする関数z_calの宣言*/
double z_cal(double **v, double *theta_h, double *w, double theta, double *xtrain, double *y, int Nh, int Dim);
/*平均二乗誤差を計算する関数cal_mseの宣言*/
double cal_mse(double **v, double *theta_h, double *w, double theta, double **xtrain, double *y, double *ytrain, int Nh, int Dim, int p);
/*正答率とconfusion_matrixを計算する関数cor_rate_and_confusion_calの宣言*/
double cor_rate_and_confusion_cal(double **v,double *theta_h,double *w,double theta,double **x,double *y_true,double *y,int Nh,int Dim,int N,double last_theta,double *confusion);



int main(int argc, char **argv){
  int Nh, Niter, Seed;
  int Ntrain, Ntest, Dim, p_Dim;
  double Lrate;
  double **v, *w;
  double *theta_h, theta;
  double **xtrain, **xtest, *ytrain, *ytest;
  int *seq_train, *seq_test;
  FILE *fp1, *fp2;
  char buf[BUF_SIZE];
  int i, j;
  double *y,z,e;
  double del_theta,del_theta_h;
  double del_v,del_w;
  int t,p;
  double cor_test , cor_train;
  double last_theta, *confusion;
  double rambda;

  
  if (argc!=13) {
    /* 引数の数が合わなければ終了 */
    printf("Usage: %s <train> <test> <# train> <# test> <input dim.> <# hidden> <learn_rate> <# iteration> <seed> <permission_Dim> <last_theta> <seisoku_λ>\n",
	   argv[0]);
    exit(1);
  }

  Ntrain = atoi(argv[3]);
  Ntest = atoi(argv[4]);
  Dim = atoi(argv[5]);
  Nh = atoi(argv[6]);
  Lrate = atof(argv[7]);
  Niter = atoi(argv[8]);
  Seed = atoi(argv[9]);
  p_Dim = atoi(argv[10]);
  last_theta =atof(argv[11]);
  rambda = atof(argv[12]);

  printf("# train_data %s\n", argv[1]);
  printf("# test_data  %s\n", argv[2]);
  printf("# Ntrain     %d\n", Ntrain);
  printf("# Ntest      %d\n", Ntest);
  printf("# Nh         %d\n", Nh);
  printf("# Lrate      %f\n", Lrate);
  printf("# Niter      %d\n", Niter);
  printf("# Seed       %d\n", Seed);
  printf("# permission_Dim       %d\n", p_Dim);
  printf("# last_theta %f\n", last_theta);
  printf("# seisoku_λ %f\n", rambda);

  /* 配列の確保 */
  xtrain = alloc_2d_double(Ntrain, Dim);
  ytrain = alloc_1d_double(Ntrain);
  xtest = alloc_2d_double(Ntest, Dim);
  ytest = alloc_1d_double(Ntest);
  v = alloc_2d_double(Nh, Dim);
  w = alloc_1d_double(Nh);
  theta_h = alloc_1d_double(Nh);
  seq_train = (int *) malloc(sizeof(int)*Ntrain);
  seq_test = (int *) malloc(sizeof(int)*Ntest);
  y=alloc_1d_double(Nh);
  confusion=alloc_1d_double(4);

  /* ファイルのオープン */
  if ((fp1=fopen(argv[1], "r"))==NULL) {
    printf("File %s open failed.\n", argv[1]);
    exit(1);
  }
  if ((fp2=fopen(argv[2], "r"))==NULL) {
    printf("File %s open failed.\n", argv[2]);
    exit(1);
  }

  /* ヘッダを捨てる */
  fgets(buf, BUF_SIZE, fp1);
  fgets(buf, BUF_SIZE, fp2);

  /* データの読み込み */
  for (i=0; i<Ntrain; i++) {
    fscanf(fp1, "%d %lf", seq_train+i, ytrain+i);
    for (j=0; j<Dim; j++)
      fscanf(fp1, "%lf", xtrain[i]+j);
  }

  for (i=0; i<Ntest; i++) {
    fscanf(fp2, "%d %lf", seq_test+i, ytest+i);
    for (j=0; j<Dim; j++)
      fscanf(fp2, "%lf", xtest[i]+j);
  }

  /*確認のための出力 */
  /*for (i=0; i<Ntrain; i++) {
    printf("TRAIN\t%d\t%d\t%.1f", i, seq_train[i], ytrain[i]);
    for (j=0; j<Dim; j++)
      printf("\t%.2f", xtrain[i][j]);
    printf("\n");
  }

  for (i=0; i<Ntest; i++) {
    printf("TEST\t%d\t%d\t%.1f", i, seq_test[i], ytest[i]);
    for (j=0; j<Dim; j++)
      printf("\t%.2f", xtest[i][j]);
    printf("\n");
    }*/
  
  /* ファイルをクローズ */
  fclose(fp1);
  fclose(fp2);
  /********************************************************************************************/
  
  srand(Seed);
  for(i=0;i<Nh;i++){
    for(j=0;j<p_Dim;j++){
      v[i][j]=-1+2.0*rand()/RAND_MAX;
    }
    theta_h[i]=-1+2.0*rand()/RAND_MAX;
    w[i]=-1+2.0*rand()/RAND_MAX;
  }
  theta=-1+2.0*rand()/RAND_MAX;

  t=0;
  while(t<Niter){
    for(p=0;p<Ntrain;p++){

      z=z_cal(v,theta_h,w,theta,xtrain[p],y,Nh,p_Dim);
      
      for(i=0;i<Nh;i++){
	del_theta_h = (z-ytrain[p])*z*(1-z)*w[i]*y[i]*(1-y[i]);   /*中間層の閾値thetaの調整量del_thetaの計算*/
	theta_h[i] = theta_h[i]+Lrate*del_theta_h;                /*中間層の閾値thetaの更新*/

	for(j=0;j<p_Dim;j++){
	  del_v = (z-ytrain[p])*z*(1-z)*w[i]*y[i]*(1-y[i])*xtrain[p][j];   /*中間層の重みvの調整量del_vの計算*/
	  v[i][j] = v[i][j]-Lrate*del_v - Lrate*rambda*v[i][j];                                   /*中間層の重みvの更新*/
	}

	del_w = (z-ytrain[p])*(1-z)*z*y[i];             /*出力層の重みwの調整量del_wの計算*/	
	w[i] = w[i]-Lrate*del_w - Lrate*rambda*w[i];                        /*出力層の重みwの更新*/
      }
      
      del_theta = (z-ytrain[p])*(1-z)*z;                /*出力層の閾値Thetaの調整量del_Thetaの計算*/
      theta = theta+Lrate*del_theta;                    /*出力層の閾値Thetaの更新*/
    }

    t++;
    /*if (t%100==0) {
      e=cal_mse(v,theta_h,w,theta,xtrain,y,ytrain,Nh,p_Dim,Ntrain);
      printf("MSE %d %f\n",t,e);*/   /*繰り返し回数、平均二乗誤差の出力（グラフ作成に用いる）*/
    /*}*/
   
    if (t%5000==0){
      /*学習データにおいてどれが間違っていてどれが正解かを出力する*/
      for(p=0;p<Ntrain;p++){       
	z=z_cal(v,theta_h,w,theta,xtrain[p],y,Nh,p_Dim);
	printf("train_t_%d  p %d z %f yout %d ytrain %d error_train %d\n",
		       t , p, z, z>=last_theta ? 1 : 0, (int) ytrain[p],
		       ((z>=last_theta && ytrain[p]==1.0) ||(z<last_theta && ytrain[p]==0.0)) ? 0 : 1);
      }
      /*学習データにおける正答率とconfusion_matrixの計算*/
      cor_train = cor_rate_and_confusion_cal(v,theta_h,w,theta,xtrain,ytrain,y,Nh,p_Dim,Ntrain,last_theta,confusion);
      printf("train_correct_late_t_%d\t%f\n",t,cor_train);/*正答率の出力*/
      
      /*printf("%f	%f\n",confusion[0],confusion[1]);*/ /*confusion_matrixの出力*/
      /*printf("%f	%f\n",confusion[2],confusion[3]);*/

      
      /*テストデータにおいてどれが間違っていてどれが正解かを出力する*/
      for(p=0;p<Ntest;p++){
	z=z_cal(v,theta_h,w,theta,xtest[p],y,Nh,p_Dim);
	printf("test_t_%d  p %d z %f yout %d ytest %d error %d\n",
	t , p , z , z>=last_theta ? 1 : 0, (int) ytest[p],
		       ((z>=last_theta && ytest[p]==1.0) || (z<last_theta && ytest[p]==0.0)) ? 0 : 1);
      }
      /*テストデータにおける正答率とconfusion_matrixの計算*/
      cor_test = cor_rate_and_confusion_cal(v,theta_h,w,theta,xtest,ytest,y,Nh,p_Dim,Ntest,last_theta,confusion);
      printf("test_correct_late_t_%d\t%f\n",t,cor_test);  /*正答率の出力*/
      
      /*printf("%f	%f\n",confusion[0],confusion[1]);*/ /*confusion_matrixの出力*/      
      /*printf("%f	%f\n",confusion[2],confusion[3]);*/
	
    }
  }
}



double y_cal(double *v, double theta_h, double *xtrain, int Dim){   /*関数y_calの定義*/
  int i;
  double y,u;
  u=0;
  for(i=0;i<Dim;i++){
    u=u+v[i]*xtrain[i];
  }
  u=u-theta_h;
  y=1.0/(1+exp(-u));      /*中間層の出力計算部分(シグモイド関数)*/
  
  return y;
}

double z_cal(double **v, double *theta_h, double *w, double theta, double *xtrain, double *y, int Nh, int Dim){     /*関数z_calの定義*/
  int i;
  double z;
  for(i=0;i<Nh;i++){
    
    y[i]=y_cal(v[i],theta_h[i],xtrain,Dim);
  }
  z=y_cal(w,theta,y,Dim);      /*出力層の出力計算部分*/
  
  return z;
}

double cal_mse(double **v, double *theta_h, double *w, double theta, double **xtrain, double *y, double *ytrain, int Nh, int Dim, int p){ /*関数cal_mseの定義*/
  int i;
  double z,e;
  
   e=0;                 /*平均二乗誤差eの初期化*/
   for(i=0;i<p;i++){
     z=z_cal(v,theta_h,w,theta,xtrain[i],y,Nh,Dim);     
     e=e+(z-ytrain[i])*(z-ytrain[i]);
   }

   e=e/p;                /*平均二乗誤差eの最終計算*/
   
   return e;
}

double cor_rate_and_confusion_cal(double **v,double *theta_h,double *w,double theta,double **x,double *y_true,double *y,int Nh,int Dim,int N,double last_theta,double *confusion){
  double z,sum;
  int p;
  double tp, fn, tn, fp;
  sum = 0;
  tp = fn = tn = fp = 0;
  
  /*正答率を計算する部分*/
  for(p=0;p<N;p++){
    z=z_cal(v,theta_h,w,theta,x[p],y,Nh,Dim);
    if((z>= last_theta && y_true[p]==1) | (z< last_theta && y_true[p]==0)){
      sum+=0;
    }else{
      sum+=1;
    }
    
  /*confusion_matrixを計算する部分*/
    if(z>= last_theta && y_true[p]==1) tp++;  /*真陽性*/
    if(z< last_theta && y_true[p]==1)  fn++;  /*偽陰性*/
    if(z< last_theta && y_true[p]==0)  tn++;  /*真陰性*/
    if(z>= last_theta && y_true[p]==0) fp++;  /*偽陽性*/  
  }
  
  confusion[0]=tp/(tp+fn)*100; /*真陽性率*/
  confusion[1]=fn/(tp+fn)*100; /*偽陰性率*/
  confusion[2]=fp/(tn+fp)*100; /*偽陽性率*/
  confusion[3]=tn/(tn+fp)*100; /*真陰性率*/
  
  return (N-sum)/N*100;/*正答率を返す*/
}



/* double型の二次元配列を確保する関数 */
double **alloc_2d_double(int m, int n){
  double **ret;
  int i;
  ret = (double **) malloc(sizeof(double *)*m);
  for (i=0; i<m; i++)
    ret[i] = (double *) malloc(sizeof(double)*n);
  return ret;
}

/* double型の一次元配列を確保する関数 */
double *alloc_1d_double(int m){
  double *ret;
  ret = (double *) malloc(sizeof(double)*m);
  return ret;
}



