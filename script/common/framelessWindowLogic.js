function getClickPos(mouseArea)
{
    mouseArea.clickPos  = Qt.point(mouseArea.mouseX , mouseArea.mouseY)
}

function updateWindowPos(mouseArea , window)
{
    var diff = Qt.point(mouseArea.mouseX - mouseArea.clickPos.x, mouseArea.mouseY - mouseArea.clickPos.y)
    window.x += diff.x;
    window.y += diff.y;
}
