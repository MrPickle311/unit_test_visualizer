function getClickPos(mouseArea)
{
    mouseArea.clickPos  = Qt.point(mouseArea.mouseX , mouseArea.mouseY)
}

function updateWindowPos(mouseArea , window)
{
    var delta = Qt.point(mouseArea.mouseX - mouseArea.clickPos.x, mouseArea.mouseY - mouseArea.clickPos.y)
    window.x += delta.x;
    window.y += delta.y;
}
