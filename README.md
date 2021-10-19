# window-profiler

|Thread ID|Name|Average|Min|Max|Call|
|:---|---:|---:|--:|--:|--:|
|32552|function 1|6.7190us|0.0000us|53.0000us|0|
|32552|function 2|7.18400us|0.0000us|56.0000us|0|
|32552|function 3|6.9440us|0.0000us|60.0000us|0|

이런식으로 각 항목들의 성능을 측정하는 툴이다.



## 사용법

```cpp
HR_PRO_BEGIN(L"function 1");

// ...

HR_PRO_END(L"function 1");
```

위와 같은 방법으로 사용하게 되며, HR_PRO_BEGIN부터 END까지 실행한 값을 저장하고, 프로그램이 종료되면 그 모든 수치를 위와 같은 식으로 저장한다.
