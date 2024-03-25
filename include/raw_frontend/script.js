document.getElementById("form").addEventListener(
    "submit",
    function()
    {
        var command = document.getElementById("command").value;
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function()
        {
            if (this.readyState == 4 && this.status == 200)
            {
                document.getElementById('command').value = "";
            }
        };
        xhttp.open("POST", "/data", true);
        xhttp.setRequestHeader(
            "Content-type",
            "application/x-www-form-urlencoded"
        );
        xhttp.send("command=" + command);
    }
);

document.getElementById("issue_command").addEventListener(
    "click",
    function()
    {
        document.getElementById("form").submit();
    }
);