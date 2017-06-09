load "capture.txt";

num_combine = 8;

len = size(capture)(1);
n = size(capture)(2);

capture(:,1)=0;


#output =zeros(len, n / num_combine);

t = 0:len-1;

for i = 1:floor(n / num_combine)
  #output(:, i) = sum(capture(:, (i-1)*num_combine+1:i*num_combine), 2);
  tmp = sum(capture(:, (i-1)*num_combine+1:i*num_combine), 2);
  
  wndw = 30;                                      %# sliding window size
  tmp2 = filter(ones(wndw,1)/wndw, 1, tmp); %# moving average
  plot(t, tmp2)
  hold on
end
hold off