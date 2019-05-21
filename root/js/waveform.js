var wavesurfer = WaveSurfer.create({
    container: '#waveform',
    waveColor: '#D2EDD4',
    progressColor: '#46B54D'
});

wavesurfer.on('ready', function () {
  var spectrogram = Object.create(WaveSurfer.Spectrogram);
  spectrogram.init({
    wavesurfer: wavesurfer,
    container: "#wave-spectrogram",
    fftSamples: 1024,
    labels: true
  });
});

wavesurfer.load('../Audios/Archive/Sounds/new.wav');
