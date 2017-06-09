
#filename = 'background.wav';
#filename = 'background-music-aac.wav';
filename = 'RichaadEB_-_Ace_Waters_-_Determination_-_01_Nascen.wav'

n = 32;
t_delta = .1;

[y, fs] = audioread(filename);

divider = 1;

y = y(1:divider:end);
fs = fs / divider;

fs

num_fft = floor((length(y) - n) / fs / t_delta);

data = zeros(num_fft, n / 2);

fout = fopen('sim_out.txt', 'w');

for i = 1 : num_fft
  start_time = t_delta * (i - 1);
  t = (1:n) + floor(start_time * fs);
  tmp=fft(y(t));
  tmp = (log(abs(tmp(1:(n / 2)))/n)+ 5)*50;
  data(i, :) = tmp;
  fprintf(fout, "%f,", start_time)
  for j = 1 : length(tmp)
    val = round(tmp(j));
    if j < length(tmp)
      fprintf(fout, "%d,", val)
    else
      fprintf(fout, "%d", val)
    end
  end
  if i < num_fft
    fprintf(fout, "\n")
  end
end
fclose(fout);
max(max(data))
min(min(data))
return

n2 = n / 2;

figure;
hold on;
names = {};
for i = 1:n2
    t = (1:num_fft) * t_delta;
    #plot(t, log(data(:, i)));
    plot(t, (data(:, i)));
    freq = (i-.5) * fs / n;
    names{i} = strcat(num2str(freq), 'Hz');
end
legend(names)
hold off;

