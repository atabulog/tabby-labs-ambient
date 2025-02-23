/**
 * This script is used to load the toolbar.html file into the toolbar-container div.
 */
document.addEventListener("DOMContentLoaded", function() {
    fetch("toolbar.html")
        .then(response => response.text())
        .then(data => {
            document.getElementById("toolbar-container").innerHTML = data;
        });
});