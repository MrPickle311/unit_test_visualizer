function findChildObject( object_name)
{

    for( var i = 0 ; i < mainWindow.contentItem.children.length; i++)
    {
        console.log("At " + i + " name is : " + mainWindow.contentItem.children[i].objectName)
        if(mainWindow.contentItem.children[i].objectName === object_name)
            return mainWindow.contentItem.children[i];
    }

    return null;
}

