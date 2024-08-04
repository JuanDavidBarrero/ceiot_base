wget -O - --method=POST \
     --header="Content-Type: application/json" \
     --body-data='{"id":"00:1A:2B:3C:4D:5E","n":"deviceTest","k":"12345"}' \
     http://localhost:8080/device
