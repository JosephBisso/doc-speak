const TEXT_COLOR = "white"
const TEXT_UNSELECTED_COLOR = "silver"
const TEXT_ACCENT_COLOR = "white"
const IMAGE_SIZE_BIG = 30
const IMAGE_SIZE_SMALL = 20
const IMAGE_SIZE_VERY_SMALL = 15

const RECT_RADIUS = 10
const FRAME_PADDING = 5

const transparentBy = (color, grad) => { return Qt.rgba(color.r, color.g, color.b, grad) }

const randomColor = () => {
    let r = Math.random()
    let g = Math.random()
    let b = Math.random()
    return Qt.rgba(r, g, b, 1)
}
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

const FONT_MEDIUM_SPECIAL = Qt.font(
    {
        bold: false,
        pointSize: 14,
        italic: true
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
