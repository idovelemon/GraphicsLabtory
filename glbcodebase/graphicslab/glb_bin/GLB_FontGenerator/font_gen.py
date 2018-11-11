from PIL import Image
from PIL import ImageFont
from PIL import ImageDraw

BMP_FONT_SIZE = 18
BMP_FONT_PADDING = 4

if __name__ == "__main__":
    bmpFontImgWidth = 0
    bmpFontImgHeight = 0

    font = ImageFont.truetype(r'C:\\Windows\\Fonts\\Calibri.ttf', BMP_FONT_SIZE)

    for value in range(32, 127):    
        width, height = font.getsize(chr(value))
        offsetx, offsety = font.getoffset(chr(value))
        bmpFontImgWidth = bmpFontImgWidth + offsetx + width + BMP_FONT_PADDING

        if height + offsety > bmpFontImgHeight:
            bmpFontImgHeight = height + offsety

    # it's better to make texture's size is power of 4
    if bmpFontImgWidth % 4 is not 0:
        bmpFontImgWidth = bmpFontImgWidth + 4 - bmpFontImgWidth % 4
    if bmpFontImgHeight % 4 is not 0:
        bmpFontImgHeight = bmpFontImgHeight + 4 - bmpFontImgHeight % 4

    img = Image.new("RGBA", (bmpFontImgWidth, bmpFontImgHeight))
    draw = ImageDraw.Draw(img)

    f = open("font.list", "w", encoding='ascii')

    x = 0
    y = 0
    for value in range(33, 127):
        offsetx, offsety = font.getoffset(chr(value))
        width, height = font.getsize(chr(value))

        draw.text((x, y), chr(value), font = font)
        print("%s %d %d %d %d" % (chr(value), offsetx + x, y, offsetx + x + width, offsety + y + height), file = f)

        # Draw border of text
        #draw.rectangle(((offsetx + x, y), (offsetx + x + width, offsety + y + height)), outline = 'white')

        x = x + offsetx + width + BMP_FONT_PADDING        

    f.close()
    
    img.save("font.bmp")