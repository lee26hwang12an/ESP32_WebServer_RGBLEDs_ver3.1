document.addEventListener("DOMContentLoaded", function()
{
    const numElement = document.getElementById("num");
    const btnModeElement = document.getElementById("btn_mode");
    const btnToggleElement = document.getElementById("btn_togg");

    let value = 0;

    btnModeElement.addEventListener("click", function()
    {
        value = (value + 1) % 11;
        numElement.textContent = value;
    });

    btnToggleElement.addEventListener("click", function()
    {
        value = 0;
        numElement.textContent = value;
    });
});