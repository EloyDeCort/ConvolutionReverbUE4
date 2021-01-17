[w,fs] = audioread('Timed_Tone_RAW.wav');
[w2,fs2] = audioread('Timed_Tone_InLoc.wav');
[w3,fs3] = audioread('Timed_Tone2_InEngine.wav');

timetableA = timetable(seconds((0:length(w)-1)'/fs),w);
timetableB = timetable(seconds((0:length(w2)-1)'/fs2),w2);
timetableC = timetable(seconds((0:length(w3)-1)'/fs3),w3);


% dur=0.5;
% winSize=512;
% overlap=[];
% fftsize=512;
% spectrogram(w2(:,1),winSize,overlap,fftsize,fs2,'yaxis')
% To hear, type soundsc(w,fs)