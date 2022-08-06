# Nucleo L152RE dev board, AM2302 + CLCD Test

점심시간에 심심해서 내가 일하고 있는 곳의 온습도를 보여줄 수 있는 것을 만들어보자! 하고 만들었네요.   
STM32 L152RE 개발보드와 캐릭터 LCD, 그리고 DHT22 (AM2302) 와  DHT11를 사용해서 테스트 했습니다.  
온습도 센서의 라이브러리는 직접 데이터시트를 보면서 만들었으나, 캐릭터 LCD제어는 사용한 모듈의 제대로 된 데이터시트를 찾기 어려워 [블로그에 올라온 자료](https://ddtxrx.tistory.com/entry/STM32HAL%EC%9D%84-%EC%82%AC%EC%9A%A9%ED%95%9C-I2C-LCD-%EC%A0%9C%EC%96%B4) 를 참고해 없는 기능을 추가해서 썼습니다.  

  
![image](https://user-images.githubusercontent.com/13059851/183253486-a364734a-5446-4a33-8362-93297d306e8f.png)
