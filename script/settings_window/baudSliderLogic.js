const baudRates = [
                   "1200",
                   "2400",
                   "4800",
                   "9600",
                   "19200",
                   "38400",
                   "57600",
                   "115200"
                ]

function getBaudRateText(sliderPos) {
    return baudRates[sliderPos]
}
