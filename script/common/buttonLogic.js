function fitScaleToState(button)
{
    return button.state === "Pressed" ? 0.94 : 1.0
}

function switchState(mouseArea , button)
{
    if (mouseArea.containsMouse)
        button.state = "Hovering"
    else
        button.state = ""
}
