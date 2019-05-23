var vol_slider = getElementById("vol_slider");
var vol_out = getElementById("vol_slider_out");

vol_slider.oninput = function() {
  vol_out.innerHTML = this.value;
} 
