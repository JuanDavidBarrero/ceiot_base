wget --method=POST \
     --body-data='{"id":"00:1A:2B:3C:4D:5E","key":"12345","t":22.5,"h":60,"p":1013}' \
     --header="Content-Type: application/json" \
     http://localhost:8080/measurement
