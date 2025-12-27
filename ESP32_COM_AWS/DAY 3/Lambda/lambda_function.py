import json
import boto3
import urllib.parse

dynamodb = boto3.resource('dynamodb')
table = dynamodb.Table('usertable')   # DynamoDB table name

def lambda_handler(event, context):
    try:
        httpmethod = event["httpMethod"]
        body = event.get("body", "")

        if httpmethod == "GET":
            return show_form()

        elif httpmethod == "POST":
            return handle_form(body)

    except Exception as e:
        return {
            "statusCode": 500,
            "body": json.dumps({"error": str(e)})
        }

# ---------- SHOW CONTACT FORM ----------
def show_form():
    with open("contactus.html", "r") as f:
        html = f.read()

    return {
        "statusCode": 200,
        "headers": {"Content-Type": "text/html"},
        "body": html
    }

# ---------- HANDLE FORM SUBMIT ----------
def handle_form(body):
    data = urllib.parse.parse_qs(body)

    table.put_item(
        Item={
            "email": data["email"][0],   # Partition Key
            "name": data["name"][0],
            "phone": data["phone"][0]
        }
    )

    with open("success.html", "r") as f:
        html = f.read()

    return {
        "statusCode": 200,
        "headers": {"Content-Type": "text/html"},
        "body": html
    }
