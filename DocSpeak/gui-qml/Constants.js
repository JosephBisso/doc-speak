const TEXT_COLOR = "white"
const TEXT_UNSELECTED_COLOR = "silver"
const TEXT_ACCENT_COLOR = "white"
const ACCENT_COLOR = "#4542f5"
const ACCENT_2_COLOR = "skyblue"
const FOREGROUND_COLOR = "lightgrey"
const BACKGROUND_COLOR = "darkgrey"
const RED_COLOR = "#e01418"
const GRAY_COLOR = "#4b4f4f"

const IMAGE_SIZE_BIG = 30
const IMAGE_SIZE_SMALL = 20

const RECT_RADIUS = 10
const FRAME_PADDING = 5

const transparentBy = (color, grad) => { return Qt.rgba(color.r, color.g, color.b, grad) }

const FONT_VERY_LARGE_BOLD = Qt.font(
    {
        bold: true,
        pointSize: 16
    }
)
const FONT_VERY_LARGE = Qt.font(
    {
        bold: false,
        pointSize: 16
    }
)
const FONT_MEDIUM = Qt.font(
    {
        bold: false,
        pointSize: 14
    }
)

const FONT_MEDIUM_BOLD = Qt.font(
    {
        bold: true,
        pointSize: 14
    }
)
const FONT_SMALL_BOLD = Qt.font(
    {
        bold: true,
        pointSize: 10
    }
)

const FONT_SMALL = Qt.font(
    {
        bold: false,
        pointSize: 10
    }
)

const FONT_VERY_SMALL = Qt.font(
    {
        bold: false,
        pointSize: 8
    }
)

const FONT_VERY_SMALL_BOLD = Qt.font(
    {
        bold: true,
        pointSize: 8
    }
)