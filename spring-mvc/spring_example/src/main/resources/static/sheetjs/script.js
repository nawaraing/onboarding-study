document.getElementById('upload').addEventListener('change', function(event) {
    const file = event.target.files[0];

    if (!file) {
        alert("file 선택이 잘못 되었습니다! : " + file);
        return;
    }

    const reader = new FileReader();

    // 파일이 ArrayBuffer로 변환될 때
    reader.onload = function(e) {
        const data = new Uint8Array(e.target.result);
        const workbook = XLSX.read(data, {
              type: 'array'
            , sheetRows: 5
            , cellNF: true
            // , cellFormula: true
            // , password: 'promisope' 
            // , dense: true
        });
        console.log("workbook.f : " + workbook.f);
        console.log("workbook.z : " + workbook.z);

        // 첫 번째 시트 가져오기
        const firstSheetName = workbook.SheetNames[0];
        const worksheet = workbook.Sheets[firstSheetName];

        // 시트 데이터를 JSON으로 변환
        const jsonData = XLSX.utils.sheet_to_json(worksheet, { header: 1 });
        var serviceLinkList = [];
        for (var i = 2; i < jsonData.length; i++) {
            const row = jsonData[i];
            console.log(row);

            var serviceLink = {
                helperNm: row[1],
                helperBirthday: row[2],
                helperGenderNm: row[3],
                clientNm: row[4],
                clientBirthday: row[5],
                clientGenderNm: row[6],
                svcStartDt: row[7],
                svcEndDt: row[8],
            };
            serviceLinkList.push(serviceLink);
        }

        const hiddenField = document.getElementById('jsonDataHidden');
        hiddenField.value = JSON.stringify(serviceLinkList);
        // console.log(hiddenField.value);
    };

    reader.onerror = function(error) {
        alert('파일을 읽는 중 오류 발생: ', error);
    };

    // 파일을 ArrayBuffer로 읽음
    reader.readAsArrayBuffer(file);
});

function sendData(event) {
    event.preventDefault();

    const hiddenField = document.getElementById('jsonDataHidden');

    if (!hiddenField) {
        alert('저장할 데이터가 없습니다.');
        return;
    }
    // console.log(hiddenField.value);

    fetch('/send-data', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json; charset=UTF-8'
        },
        body: hiddenField.value
    })
    .then(response => response.json())
    .then(result => {
        console.log('서버 응답:', result);
        alert('데이터가 성공적으로 저장되었습니다!');
    })
    .catch(error => {
        console.error('데이터 저장 중 오류 발생:', error);
        alert('데이터 저장 중 오류가 발생했습니다.');
    });
}
