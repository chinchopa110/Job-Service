curl -X POST http://localhost:8080/api/employers \
  -H "Content-Type: application/json" \
  -d '{"companyName":"TechCorp","email":"contact@techcorp.com","description":"Leading tech company","city":"Moscow"}'

curl -X GET http://localhost:8080/api/employers

curl -X POST http://localhost:8080/api/workers \
  -H "Content-Type: application/json" \
  -d '{"name":"Ivan Petrov","email":"ivan@example.com","skills":["C++","Python"],"experience":5,"city":"Moscow","resume":"Experienced developer"}'

curl -X POST http://localhost:8080/api/jobs \
  -H "Content-Type: application/json" \
  -d '{"employerId":1,"title":"Senior C++ Developer","description":"We need experienced C++ developer","salary":200000,"city":"Moscow","isRemote":false,"profession":"Software Engineer"}'

curl -X GET "http://localhost:8080/api/jobs/search"
curl -X GET "http://localhost:8080/api/jobs/search?city=Moscow&minSalary=150000"

curl -X POST http://localhost:8080/api/replies \
  -H "Content-Type: application/json" \
  -d '{"jobPostingId":1,"workerId":1}'

curl -X GET http://localhost:8080/api/replies/job/1
