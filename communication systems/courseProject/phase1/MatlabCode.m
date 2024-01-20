% Parameters
Am = input(' (Am): ');
Ac = input(' (Ac): ');
Fm = input(' (Fm): ');
Fc = input(' (Fc): ');
ka = input(' (ka): ');
Tc = 1/Fc; % Set Tc as the inverse of the carrier frequency

% Time vector
t = 0:0.000001:1; % from 0 to 1 second with a step of 1 ms

% Generate signals
mt = Am * cos(2 * pi * Fm * t); % m(t) 
ct = Ac * cos(2 * pi * Fc * t); % c(t) 
st = (1 + ka * mt) .* ct;

% Periodic Pulse signal
pt = rectpuls(mod((2 * t), Tc)); % p(t)

% Frequency vector
Fs = 1/(t(2)-t(1));
frequencies = linspace(-Fs/2, Fs/2, length(t));

% Plot m(t) and m(f)
figure;
subplot(2,1,1);
plot(t, mt, 'b', 'LineWidth', 1.5);
title('Message Signal (m(t))');
xlabel('Time (s)');
ylabel('Amplitude');
grid on;

subplot(2,1,2);
spectrum_m = abs(fftshift(fft(mt))) / length(t); % Normalize the magnitude
plot(frequencies, spectrum_m, 'b', 'LineWidth', 1.5);
title('Frequency Domain - Message Signal (M(f))');
xlabel('Frequency (Hz)');
ylabel('Magnitude');
grid on;

% Plot c(t) and c(f)
figure;
subplot(2,1,1);
plot(t, ct, 'r', 'LineWidth', 1.5);
title('Carrier Signal (c(t))');
xlabel('Time (s)');
ylabel('Amplitude');
grid on;

subplot(2,1,2);
spectrum_c = abs(fftshift(fft(ct))) / length(t); % Normalize the magnitude
plot(frequencies, spectrum_c, 'r', 'LineWidth', 1.5);
title('Frequency Domain - Carrier Signal (C(f))');
xlabel('Frequency (Hz)');
ylabel('Magnitude');
grid on;

% Plot p(t) and p(f)
figure;
subplot(2,1,1);
plot(t, pt, 'm', 'LineWidth', 1.5);
title('Periodic Pulse Signal (p(t))');
xlabel('Time (s)');
ylabel('Amplitude');
grid on;

subplot(2,1,2);
spectrum_p = abs(fftshift(fft(pt))) / length(t); % Normalize the magnitude
plot(frequencies, spectrum_p, 'm', 'LineWidth', 1.5);
title('Frequency Domain - Periodic Pulse Signal (P(f))');
xlabel('Frequency (Hz)');
ylabel('Magnitude');
grid on;

% Plot s(t) and s(f)
figure;
subplot(2,1,1);
plot(t, st, 'g', 'LineWidth', 1.5);
title('Modulated Signal (s(t))');
xlabel('Time (s)');
ylabel('Amplitude');

subplot(2,1,2);
spectrum_s = abs(fftshift(fft(st))) / length(t); % Normalize the magnitude
plot(frequencies, spectrum_s, 'g', 'LineWidth', 1.5);
title('Frequency Domain - Modulated Signal (S(f))');
xlabel('Frequency (Hz)');
ylabel('Magnitude');
grid on;
