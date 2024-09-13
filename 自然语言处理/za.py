import re   #正则表达式
import string
import nltk
import pandas as pd
from sklearn.feature_extraction.text import TfidfVectorizer
from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize

from sklearn.model_selection import train_test_split,GridSearchCV
from sklearn.naive_bayes import MultinomialNB#先验概率为多项式分布
from sklearn.svm import SVC
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, confusion_matrix, roc_auc_score

nltk.download('stopwords')
nltk.download('punkt')
stop_words = set(stopwords.words('english'))


# 加载数据
with open('mr.txt', 'r',encoding='utf-8',errors='ignore') as f:
    features = f.readlines()
    X_train=features[0:7108]
    X_test=features[7108:]

with open('mr_labels.txt', 'r',encoding='utf-8',errors='ignore') as f:
        labels = f.readlines()
        labels=[int(s.rsplit('\t',1)[-1].strip('\n'))for s in labels]
        Y_train=labels[0:7108]
        Y_test=labels[7108:]


# 数据清洗函数
def clean_text(text):

        # 去除标点符号
        text = text.translate(str.maketrans('', '', string.punctuation))
        # 转换为小写
        text = text.lower()
        # 去除停用词
        words = word_tokenize(text)#对字符串进行分词
        text= [word for word in words if word not in stop_words]#去除停用词
        text = ' '.join(text)#重新组合为字符串
        return text

# 清洗数据
X_train = [clean_text(text) for text in X_train]
X_test=  [clean_text(text) for text in X_test]

#特征提取
vectorizer = TfidfVectorizer()
X_train = vectorizer.fit_transform(X_train)
X_test = vectorizer.transform(X_test)
#获取验证集
X_train, X_val, Y_train, Y_val = train_test_split(X_train, Y_train, test_size=0.2, random_state=42)


# 定义评估模型性能的函数
def evaluate_model(predictions, true_labels, model, X_val):
    accuracy = accuracy_score(true_labels, predictions)
    precision = precision_score(true_labels, predictions, average='weighted')
    recall = recall_score(true_labels, predictions, average='weighted')
    f1 = f1_score(true_labels, predictions, average='weighted')
    roc_auc = roc_auc_score(true_labels, model.predict_proba(X_val)[:,1])
    conf_matrix = confusion_matrix(true_labels, predictions)

    print(f'Accuracy: {accuracy}')
    print(f'Precision: {precision}')
    print(f'Recall: {recall}')
    print(f'F1 Score: {f1}')
    print(f'ROC AUC Score: {roc_auc}')
    print(f'Confusion Matrix:\n {conf_matrix}')

# 超参数调整 - Naive Bayes
nb_model = MultinomialNB()
nb_params = {'alpha': [0.1, 0.5, 1.0, 5.0, 10.0]}
nb_grid = GridSearchCV(nb_model, nb_params, cv=5, scoring='accuracy')
nb_grid.fit(X_train, Y_train)
best_nb_model = nb_grid.best_estimator_

# 超参数调整 - SVM
svm_model = SVC(probability=True)
svm_params = {'C': [0.1, 1, 10], 'kernel': ['linear', 'rbf']}
svm_grid = GridSearchCV(svm_model, svm_params, cv=5, scoring='accuracy')
svm_grid.fit(X_train, Y_train)
best_svm_model = svm_grid.best_estimator_


#在验证集上评估模型
nb_val_predictions = best_nb_model.predict(X_val)
print("\nNaive Bayes Model Validation Performance:")
evaluate_model(nb_val_predictions, Y_val, best_nb_model, X_val)


#在验证集上评估模型
svm_val_predictions = best_svm_model.predict(X_val)
print("\nSVM Model Validation Performance:")
evaluate_model(svm_val_predictions, Y_val, best_svm_model, X_val)


#在测试集上评估nb
nb_test_predictions = best_nb_model.predict(X_test)
print("\nNaive Bayes Model Test Performance:")
evaluate_model(nb_test_predictions, Y_test, best_nb_model, X_test)



#在测试集上评估svm
svm_test_predictions = best_svm_model.predict(X_test)
print("\nSVM Model Test Performance:")
evaluate_model(svm_test_predictions, Y_test, best_svm_model, X_test)

