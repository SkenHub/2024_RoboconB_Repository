clear

// ステップ 1：連立 1 次方程式の係数の入力 //////////////////////
printf("t_max = ?\n"); t_max = scanf("%d");
printf("\n")

for i = 1:3   // A の要素の入力
  a(1,i) = t_max.^(4-i);
  //printf("%f ",a(1,i));
end
//printf("\n");
for i = 1:3   // A の要素の入力
  a(2,i) = (t_max.^(5-i))/(5-i);
  //printf("%f ",a(2,i));
end
//printf("\n");
for i = 1:3   // A の要素の入力
  a(3,i) = (t_max.^(6-i))/((6-i)*(5-i));
  //printf("%f ",a(3,i));
end
//printf("\n");
  
printf("x_0 = ?\n"); x_0 = scanf("%f");
printf("\n")
printf("x_max = ?\n"); x_max = scanf("%f");
printf("\n")

b(1) = 0;
b(2) = 0;
b(3) = x_max - x_0;
n  = 3;
// ステップ 2 ///////////////////////////////////////////////
for k = 1:n-1
  // ステップ 2-1 の計算：行交換による部分ピボット操作（ここから）//
  a_max = a(k,k);   // ステップ i 
  i_max = k;
  
  for i = k+1:n     // ステップ ii 
    if abs(a(i,k)) > abs(a_max)
      a_max = a(i,k);
      i_max = i;
    end
  end
  
  if i_max ~= k     // ステップ iii
    for j = k:n
      a_temp(j)  = a(k,j);
      a(k,j)     = a(i_max,j);
      a(i_max,j) = a_temp(j);
    end
    
    b_temp   = b(k);
    b(k)     = b(i_max);
    b(i_max) = b_temp;
  end
  // ステップ 2-1 の計算（ここまで）////////////////////////////

  // ステップ 2-2 の計算（ここから）////////////////////////////
  for j = k+1:n
    a(k,j) = a(k,j)/a(k,k);
  end
  b(k) = b(k)/a(k,k);
  
  for i = 1:n
    if i ~= k
      for j = k+1:n
        a(i,j) = a(i,j) - a(k,j)*a(i,k);
      end
      b(i) = b(i) - b(k)*a(i,k);
    end
  end
  // ステップ 2-2 の計算（ここまで）////////////////////////////
end

// ステップ 3 ///////////////////////////////////////////////
b(n) = b(n)/a(n,n);
for i = 1:n-1
  b(i) = b(i) - b(n)*a(i,n);
end

printf("\n");
for i = 1:n
  printf("a%d = %f\n", i, b(i));
end

t = 0:0.001:t_max;
x_1 = (b(1)/20)*t.^5 + (b(2)/12)*t.^4 + (b(3)/6)*t.^3;
x_2 = (b(1)/4 )*t.^4 + (b(2)/3 )*t.^3 + (b(3)/2)*t.^2;
x_3 = (b(1)   )*t.^3 + (b(2)   )*t.^2 + (b(3)  )*t;

subplot(3,1,1);
xgrid();
plot(t,x_1);
subplot(3,1,2);
xgrid();
plot(t,x_2);
subplot(3,1,3);
xgrid();
plot(t,x_3);



