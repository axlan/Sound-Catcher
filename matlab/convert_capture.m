load "capture1.txt";

Fs = 22191;
scale = 2;
out_period = .1;

t= capture1(:,1)/Fs;
start_t = t(1);
data = capture1(:,2:end) * scale;

[num_fft, num_bins] = size(data)

fout = fopen('sim_out.txt', 'w');

last = 0;

max(max(data))

cur_t = 0;

while 1
  
  cur_t += out_period;
  
  idx = find(t > (cur_t + start_t), 1);
  if isempty(idx)
    break
  endif
  
  bins = mean(data(1:idx,:));
  data = data(idx:end,:);
  t = t(idx:end,:);
  
  fprintf(fout, "%f,", cur_t)
  for j = 1 : num_bins
    val = bins(j);
    if j < num_bins
      fprintf(fout, "%d,", val)
    else
      fprintf(fout, "%d", val)
    endif
  endfor
  if i < num_fft
    fprintf(fout, "\n")
  endif
endwhile

fclose(fout);
