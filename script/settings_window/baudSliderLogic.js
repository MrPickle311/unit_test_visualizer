const baudRates = ["300",
                   "1200",
                   "4800",
                   "9600",
                   "19200",
                   "38400",
                   "57600",
                   "115200",
                   "230400",
                   "460800",
                   "921600"]

function getBaudRateText(sliderPos) {
    return baudRates[sliderPos]
}
