#ifndef _CSS_H_
#define _CSS_H_


const char *style_css = R"CSS(



* {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
    font-family: 'Nexa', sans-serif;
}

body {
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    min-height: 100vh;
    background-color: #e9e9e9;
}

#num {
    user-select: none;
    position: absolute;
    transform: translate(0, -100%);
    font-size: 100px;
    font-weight: 100;
    text-align: center;
    color: #535353;
}

#btn_mode {
    position: absolute;
    width: 600px;
    height: 100px;
    border: none;
    border-radius: 15px;
    background-color: #e9e9e9;
    box-shadow: 10px 10px 10px rgba(0, 0, 0, 0.03),
                -10px -10px 10px rgba(255, 255, 255, 0.3);
    font-size: 35px;
    font-weight: 600;
    text-align: center;
    color: #535353;
    transition: all 0.25s ease;
}

#btn_mode:active {
    transform: scale(0.98);
    background-color: #e4e4e4;
    box-shadow: 10px 10px 10px rgba(0, 0, 0, 0.0),
                -10px -10px 10px rgba(255, 255, 255, 0.0);
    color: #00c975;
}

#btn_togg {
    position: absolute;
    width: 600px;
    height: 100px;
    transform: translate(0, 120%);
    border: none;
    border-radius: 15px;
    background-color: #e9e9e9;
    box-shadow: 10px 10px 10px rgba(0, 0, 0, 0.03),
                -10px -10px 10px rgba(255, 255, 255, 0.3);
    font-size: 35px;
    font-weight: 600;
    text-align: center;
    color: #535353;
    transition: all 0.25s ease;
}

#btn_togg:active {
    transform: translate(0, 120%) scale(0.98);
    background-color: #e4e4e4;
    box-shadow: 10px 10px 10px rgba(0, 0, 0, 0.0),
                -10px -10px 10px rgba(255, 255, 255, 0.0);
    color: #c90043;
}



)CSS";


#endif /* EOF _CSS_H_*/