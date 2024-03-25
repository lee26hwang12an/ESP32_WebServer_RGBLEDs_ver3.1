#ifndef _HTML_H_
#define _HTML_H_


const char *index_html = R"HTML(



<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>LED CONTROL PANEL</title>
    <link rel="stylesheet" href="/style_css">
</head>

<body>
    <form action="" id="form">
        <input type="text" id="command">
        <button id="issue_command">Issue Command</button>
    </form>

    <script src="/script_js"></script>
</body>

</html>



)HTML";


#endif /* EOF _HTML_H_ */